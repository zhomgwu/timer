#include "timer_list.h"

CTimerNode::CTimerNode()
	: m_pNext(0)
	, m_uExpire(0) 
	, m_uDelay(0)
	, m_TimerCall(0) {
}

CTimerNode::~CTimerNode() {
}

CTimerNode *
CTimerNode::getPrev() {
	return m_pPrev;
}

void 
CTimerNode::setPrev(CTimerNode *node) {
	m_pPrev = node;
}

CTimerNode*
CTimerNode::getNext() {
	return m_pNext;
}

void
CTimerNode::setNext(CTimerNode *node) {
	m_pNext = node;
}

CTimerList *
CTimerNode::getOwnerList() {
	return m_pOwnerList;
}

void 
CTimerNode::setOwnerList(CTimerList *list) {
	m_pOwnerList = list;
}

UINT32_ 
CTimerNode::getExpire() {
	return m_uExpire;
}

void 
CTimerNode::setExpire(UINT32_ expire) {
	m_uExpire = expire;
}

void 
CTimerNode::setDelay(UINT32_ delay) {
	m_uDelay = delay;
}

void 
CTimerNode::setTimerCall(TimerCall callback) {
	m_TimerCall = callback;
}

void 
CTimerNode::execute() {
	if (m_TimerCall) {
		(*m_TimerCall)(m_uDelay);
	}
}

CTimerList::CTimerList()
	:m_Head()
	,m_pTail(0) {
}

CTimerList::~CTimerList() {
}

CTimerNode *
CTimerList::clear() {
	CTimerNode *pNode = m_Head.getNext();
	m_Head.setPrev(0);
	m_Head.setNext(0);
	m_pTail = &m_Head;
	return pNode;
}

void 
CTimerList::link(CTimerNode *node) {
	node->setOwnerList(this);
	node->setPrev(m_pTail);
	m_pTail->setNext(node);
	m_pTail = node;
	node->setNext(0);
}

void 
CTimerList::remove(CTimerNode *node) {
	if (node != &m_Head) {
		if (node == m_pTail) {
			node->getPrev()->setNext(0);
			m_pTail = node->getPrev();
		}
		else {
			node->getPrev()->setNext(node->getNext());
			node->getNext()->setPrev(node->getPrev());
		}
		node->setOwnerList(0);
	}
}

CTimerNode *
CTimerList::getHead() {
	return &m_Head;
}

CTimerNode *
CTimerList::getTail() {
	return m_pTail;
}
