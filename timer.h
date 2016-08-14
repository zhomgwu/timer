#ifndef __TIMER_H__
#define __TIMER_H__

#define TIMER_NEAR_SHIFT 8
#define TIMER_NEAR (1<<TIMER_NEAR_SHIFT)
#define TIMER_LEVEL_SHIFT 6
#define TIMER_LEVEL (1<<TIMER_LEVEL_SHIFT)
#define TIMER_NEAR_MASK (TIMER_NEAR - 1)
#define TIMER_LEVEL_MASK (TIMER_LEVEL - 1)


#include "timer_list.h"
#include "type_def.h"

class CTimer
{
private:

	CTimer();
	~CTimer();

public:

	static CTimer *getInstance();
	static void destroy();
	
	void init();
	void update();
	void execute();
	CTimerNode* timeOut(UINT32_ cs, TimerCall callback = 0);
	void stop(CTimerNode *node);

	UINT32_ getStart();
	UINT64_ getNow();
	UINT64_ getTime();

private:

	void shift();
	void addToList(CTimerNode *node);
	void moveList(int level, int index);
	void updateOnce();
	// if you want to avoid memory fragmentation, you can use cache here.
	CTimerNode *newNode();
	void deleteNode(CTimerNode *node);

private:

	static CTimer *m_pInstance;
	UINT32_ m_uTick;
	UINT32_ m_uStart;
	UINT64_ m_uCurrent;
	UINT64_ m_uCurrentPoint;
	CTimerList m_TimerNear[TIMER_NEAR];
	CTimerList m_TimerLevel[4][TIMER_LEVEL];
};

#endif //__TIMER_H__
