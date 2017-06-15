#define FOSC           16000
#define F_CPU          16000000UL
#include "hcsr04.h"
#include "timers.h"
#include <util/delay.h>

Hcsr04::Hcsr04(const Output& trigpin, const Input& echopin) :
    _trigger(trigpin),
    _echo(echopin),
    _timer(Timer::TIMER_PRESCALER_64)
{
}

uint16_t Hcsr04::getDistance(uint16_t sound_speed) {

    uint16_t distance=0;
    char end=0;
    uint32_t temps = 0;

    _trigger.setHigh();
    _delay_us(20);
    _trigger.setLow();

    _timer.reset();
    _timer.start();
    while(!end){
        if(_echo.read()) end=1;
        else if(_timer.getTicks()>500) end=2;
    }
    _timer.stop();

    if(end==1){
        end = 0;
        _timer.reset();
        _timer.start();


        while(!end){// Wait not echo
            if(!_echo.read()) end=1;
            else if(_timer.getTicks()>10000) end=2;
        }

        _timer.stop();

        if(end==1){
            temps = _timer.getTicks();
            distance = temps * (sound_speed / 500.0);
        }else{
            distance=2;
        }
    }else{
        distance=1;
    }
	return distance;
}
