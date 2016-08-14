#ifndef __TIMER_LIST_H__
#define __TIMER_LIST_H__

#include "type_def.h"

typedef void (*TimerCall)(UINT32_ dt);

class CTimerList;
class CTimerNode
{
public:

	CTimerNode();
	~CTimerNode();

public:

	CTimerNode *getPrev();
	void setPrev(CTimerNode *node);
	CTimerNode *getNext();
	void setNext(CTimerNode *node);
	CTimerList *getOwnerList();
	void setOwnerList(CTimerList *list);
	UINT32_ getExpire();
	void setExpire(UINT32_ expire);
	void setDelay(UINT32_ delay);
	void setTimerCall(TimerCall callback);
	void execute();

private:

	CTimerNode *m_pPrev;
	CTimerNode *m_pNext;
	CTimerList *m_pOwnerList;
	UINT32_		m_uExpire;
	UINT32_		m_uDelay;
	TimerCall	m_TimerCall;
};

class CTimerList
{
public:

	CTimerList();
	~CTimerList();

public:

	CTimerNode* clear();
	void link(CTimerNode *node);
	void remove(CTimerNode *node);

	CTimerNode *getHead();
	CTimerNode *getTail();

private:

	CTimerNode m_Head;
	CTimerNode *m_pTail;
};

#endif __TIMER_LIST_H__
