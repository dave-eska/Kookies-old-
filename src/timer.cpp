#include"timer.h"

#include<raylib.h>
#include<raymath.h>

void startTimer(Timer* timer, float lifeTime){
    if(timer != NULL){
        timer->lifeTime = lifeTime;
    }
}

void updateTimer(Timer* timer)
{
    // subtract this frame from the timer if it's not allready expired
    if (timer != NULL && timer->lifeTime > 0)
        timer->lifeTime -= GetFrameTime();
}

// check if a timer is done.
bool timerDone(Timer* timer)
{
    if (timer != NULL)
        return timer->lifeTime <= 0;

	return false;
}
