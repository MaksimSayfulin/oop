#include <iostream>
#include <exception>
#include "Car.h"

bool IsAvailableSpeed(int speed, Gear gear)
{
	switch (gear)
	{
	case Gear::Reverse:
		return speed >= 0 && speed <= 20;
	case Gear::First:
		return speed >= 0 && speed <= 30;
	case Gear::Second:
		return speed >= 20 && speed <= 50;
	case Gear::Third:
		return speed >= 30 && speed <= 60;
	case Gear::Fourth:
		return speed >= 40 && speed <= 90;
	case Gear::Fifth:
		return speed >= 50 && speed <= 150;
	}

	return true;
}

Car::Car()
{
	isEngineTurnedOn = false;
	currentGear = Gear::Neutral;
	speed = 0;
}

void Car::TurnOnEngine()
{
	if (!isEngineTurnedOn)
	{
		isEngineTurnedOn = true;
	}
	else
	{
		throw std::runtime_error("Engine is alredy started");
	}
}

void Car::TurnOffEngine()
{
	if (isEngineTurnedOn && currentGear == Gear::Neutral && speed == 0)
	{
		isEngineTurnedOn = false;
	}
	else
	{
		throw std::runtime_error("Engine can't be turned off");
	}
}

void Car::SetSpeed(int newSpeed)
{
	if (currentGear == Gear::Neutral && newSpeed > speed)
	{
		throw std::runtime_error("Engine can't accelerate on neutral gear");
	}

	if (IsAvailableSpeed(newSpeed, currentGear))
	{
		speed = newSpeed;
	}
	else
	{
		throw std::invalid_argument("This speed can't be set for this gear");
	}
}

void Car::SetGear(Gear gear)
{
	if (!isEngineTurnedOn)
	{
		throw std::invalid_argument("Cannot set gear while engine is off");
	}
	if (gear == Gear::Reverse && speed != 0)
	{
		throw std::invalid_argument("Cannot reverse while moving");
	}
	if (gear > Gear::Neutral && currentGear == Gear::Reverse && speed != 0)
	{
		throw std::invalid_argument("Cannot set forward gear while reversing");
	}
	if (IsAvailableSpeed(speed, gear))
	{
		currentGear = gear;
	}
	else
	{
		throw std::invalid_argument("Unsuitable current speed");
	}
}

