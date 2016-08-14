#include "timer.h"

#if defined(_WIN32)
#include <WinSock2.h>
#endif

CTimer::CTimer()
	: m_uTick(0)
	, m_uStart(0)
	, m_uCurrent(0)
	, m_uCurrentPoint(0) {
}

CTimer::~CTimer() {
}

CTimer* 
CTimer::m_pInstance = 0;

CTimer *
CTimer::getInstance() {
	if (m_pInstance == 0) {
		m_pInstance = new CTimer;
	}
	return m_pInstance;
}

void 
CTimer::destroy() {
	if (m_pInstance != 0) {
		delete m_pInstance;
	}
}

void 
CTimer::init() {
	m_uTick = 0;
	m_uStart = 0;
	m_uCurrent = m_uCurrentPoint = getTime();
	for (int i = 0; i < TIMER_NEAR; ++i) {
		m_TimerNear[i].clear();
	}

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < TIMER_LEVEL; ++j) {
			m_TimerLevel[i][j].clear();
		}
	}
}

void 
CTimer::update() {
	UINT64_ cp = getTime();
	if (cp < m_uCurrentPoint) {
		m_uCurrentPoint = cp;
		return;
	}
	else if (cp != m_uCurrentPoint) {
		int diff = (int)(cp - m_uCurrentPoint);
		m_uCurrentPoint = cp;
		m_uCurrent += diff;
		for (int i = 0; i < diff; ++i) {
			updateOnce();
		}		
	}
}

void 
CTimer::execute() {
	int index = m_uTick & TIMER_NEAR_MASK;
	CTimerNode *pNode = m_TimerNear[index].getHead()->getNext();
	while (pNode) {
		CTimerNode *pTemp = pNode->getNext();
		pNode->execute();
		deleteNode(pNode);
		pNode = pTemp;
	}
	m_TimerNear[index].clear();
}

CTimerNode*
CTimer::timeOut(UINT32_ cs, TimerCall callback) {
	CTimerNode *pNode = newNode();
	pNode->setExpire(m_uTick + cs);
	pNode->setDelay(cs);
	pNode->setTimerCall(callback);
	addToList(pNode);
	return pNode;
}

void 
CTimer::stop(CTimerNode *node) {
	CTimerList *pTimerList = node->getOwnerList();
	pTimerList->remove(node);
	deleteNode(node);
}

UINT32_ 
CTimer::getStart() {
	return m_uStart;
}

UINT64_
CTimer::getNow() {
	return m_uCurrent;
}

UINT64_ 
CTimer::getTime() {
	UINT64_ t;
#if defined(__APPLE__)
	struct timespec ti;
	clock_gettime(CLOCK_MONOTONIC, &ti);
	t = (UINT64)ti.tv_sec * 100;
	t += ti.tv_nsec / 10000000;
#endif

#if defined (__LINUX__)
	struct timeval tv;
	gettimeofday(&tv, NULL);
	t = (UINT64)tv.tv_sec * 100;
	t += tv.tv_usec / 10000;
#endif

#if defined(_WIN32)
	LARGE_INTEGER liTime, liFreq;
	QueryPerformanceFrequency( &liFreq );
	QueryPerformanceCounter( &liTime );
	t = (UINT64_)( liTime.QuadPart * 100.0 / liFreq.QuadPart);
#endif
	return t;
}

void 
CTimer::updateOnce() {
	execute();
	shift();
	execute();
}

void 
CTimer::shift() {
	UINT32_ tick = ++m_uTick;
	if (tick == 0) {
		moveList(3, 0);
	}
	else {
		UINT32_ mask = TIMER_NEAR;
		UINT32_ time = tick >> TIMER_NEAR_SHIFT;
		int i = 0;
		while ((tick & (mask-1)) == 0) {
			int index = time & TIMER_LEVEL_MASK;
			if (index != 0) {
				moveList(i, index);
				break;
			}
			time >>= TIMER_LEVEL_SHIFT;
			mask <<= TIMER_LEVEL_SHIFT;
			++i;
		}
	}
}

void 
CTimer::addToList(CTimerNode *node) {
	UINT32_ expire = node->getExpire();
	UINT32_ mask = TIMER_NEAR;
	if ((expire | (mask - 1)) == (m_uTick | (mask - 1))) {
		int index = expire & TIMER_NEAR_MASK;
		m_TimerNear[index].link(node);
	}
	else {
		mask = TIMER_NEAR << TIMER_LEVEL_SHIFT;
		int i = 0;
		for (; i < 3; ++i) {
			if ((expire | (mask - 1)) == (m_uTick | (mask - 1))) {
				break;
			}
			mask <<= TIMER_LEVEL_SHIFT;
		}

		int index = (expire >> (TIMER_NEAR_SHIFT + i * TIMER_LEVEL_SHIFT)) & TIMER_LEVEL_MASK;
		m_TimerLevel[i][index].link(node);
	}
}

void 
CTimer::moveList(int level, int index) {
	CTimerNode *pNode = m_TimerLevel[level][index].clear();
	while (pNode) {
		CTimerNode *pTemp = pNode->getNext();
		addToList(pNode);
		pNode = pTemp;
	}
}

CTimerNode *
CTimer::newNode() {
	return new CTimerNode;
}

void 
CTimer::deleteNode(CTimerNode *node) {
	delete node;
}
