#ifndef HCSR04_H
#define HCSR04_H

//! \file hcsr04.h
//! \brief Hcsr04 class. 
//! \author Remy Guyonneau, Franck Mercier, Philippe Lucidarme
//! \date 2017 04 24

#include "output.h"
#include "input.h"
#include "timers.h"


//! \class Hcsr04
//! \brief Hcsr04 class. 
//!
//! HCSRO4 class. This class implements the behavior of the HCSRO4 ultrasonic range finder.
class Hcsr04
{
public:

    //! \brief Constructor for an HCSR04
    //!
    //! Constructor for an HCSR04
    //!
    //! \param[in] trigpin : The trigger pin
    //! \param[in] echopin : The echo receiving pin
    Hcsr04(const Output& trigpin, const Input& echopin);

    //! \brief Make a distance measure
    //! Make a distance measure. Not Blocking! A timer is used to avoid blocking the function (if the sound wave does not come back)
    //! \param[in] sound_speed : Sound speed in air (m/s).
    //! \return The distance in mm or an error message (1 or 2 meaning that the wave sound has had some issues)
    uint16_t getDistance(uint16_t sound_speed=340);

private:

    //! \brief trigger pin
    Output _trigger;

    //! \brief echo pin
    Input _echo;

    //! \brief timer
    //! Timer to avoid blocking getDistance()
    Timer16 _timer;
};

#endif // HCSR04_H
