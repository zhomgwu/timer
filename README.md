# timer
implements with cpp

# usage

include "timer.h" first, and you must call 
```
CTimer::getInstance()->update();
```
in you main loop on you application.

use 
```
CTimer::getInstance()->timeOut(100, timeOutFunc);
```
create a timernode, you can store the return values if you want to stop it before time out, use
```
CTimer::getInstance()->stop(timernode);
```
