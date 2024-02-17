#pragma once

typedef struct{
    float lifeTime;
}Timer;

void startTimer(Timer* timer, float lifeTime);
void updateTimer(Timer* timer);
bool timerDone(Timer* timer);
