#pragma once

enum class Gear
{
	Reverse = -1,
	Neutral = 0,
	First = 1,
	Second = 2,
	Third = 3,
	Fourth = 4,
	Fifth = 5
};


class Car
{
public:
	Car();

	void TurnOnEngine();
	void TurnOffEngine();
	void SetGear(Gear gear);
	void SetSpeed(int speed);

	int GetSpeed() const { return speed; }
	Gear GetGear() const { return currentGear; }
	bool GetEngineIsTurnedOn() const { return isEngineTurnedOn; }

private:
	bool isEngineTurnedOn;
	Gear currentGear;
	int speed;

};