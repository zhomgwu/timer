
#include <stdio.h>
#include "timer.h"

CTimerNode * timer1 = 0;
CTimerNode * timer2 = 0;

void timeOutPrint(UINT32_ cs) {
	printf("time out cs=%d now=%d\n", cs, CTimer::getInstance()->getNow());
	if ( cs == 400) {
		//annotation this line second timer will print after 10s. 
		CTimer::getInstance()->stop(timer2);
	}
	CTimer::getInstance()->timeOut(600, timeOutPrint);
}

int main() {

	CTimer::getInstance()->init();
	timer1 = CTimer::getInstance()->timeOut(400, timeOutPrint);
	timer2 = CTimer::getInstance()->timeOut(1000, timeOutPrint);
	for (;;) {
		CTimer::getInstance()->update();
	}
	return 0;
}
