#include "CarDriver.h"
#include <iostream>
#include <string>
#include <sstream> 

enum class CommandType
{
	EngineOn,
	EngineOff,
	SetGear,
	SetSpeed,
	Info,
	Unknown,
	Help
};
//Определить и реалищовать

void PrintInfo(Car car, std::ostream& out)
{
	std::string engineState = (car.GetEngineIsTurnedOn()) ? "on" : "off";
	out << "Engine: " << engineState << std::endl;

	std::string direction;
	Gear carGear = car.GetGear();
	switch (carGear)
	{
	case Gear::Reverse:
		direction = "backward";
		break;
	case Gear::Neutral:
		direction = "still standing";
		break;
	default:
		direction = "forward";
		break;
	}

	out << "Direction: " << direction << std::endl;
	out << "Speed: " << car.GetSpeed() << std::endl;
	out << "Gear: " << static_cast<int>(carGear) << std::endl;
}

CommandType ParseCommand(const std::string& command)
{
	std::stringstream ss(command);
	std::string cmd;
	ss >> cmd;

	if (cmd == "EngineOn") return CommandType::EngineOn;
	if (cmd == "EngineOff") return CommandType::EngineOff;
	if (cmd == "SetGear") return CommandType::SetGear;
	if (cmd == "SetSpeed") return CommandType::SetSpeed;
	if (cmd == "Info") return CommandType::Info;
	return CommandType::Unknown;
}

void DoCommand(Car& car, CommandType type, const std::string& command)
{
	switch(type) //разбить
	{
	case CommandType::EngineOn:
		car.TurnOnEngine();
		break;
	case CommandType::EngineOff:
		car.TurnOffEngine();
		break;
	case CommandType::SetGear:
	{
		std::stringstream ss(command);
		std::string garbage; 
		int gear;
		if (ss >> garbage >> gear) 
		{
			if (gear > 5 || gear < -1)
			{
				throw std::invalid_argument("Unsuitable current speed");
			}
			car.SetGear(static_cast<Gear>(gear));
		}
		break;
	}
	case CommandType::SetSpeed:
	{
			
		std::stringstream ss(command);
		std::string garbage;
		int speed;
		if (ss >> garbage >> speed) 
		{
			car.SetSpeed(speed);
		}
		break;
	}
	case CommandType::Info:
		PrintInfo(car, std::cout);
		break;
	case CommandType::Unknown:
		throw std::invalid_argument("Unknown command");
		break;
	}
		
}

void CarDriver::CarDrive(std::istream& in) // отличие композиции от агрегации(время жизни)
{
	Car car{};

	std::string command;
	
	while (std::getline(in, command))
	{
		try
		{
			CommandType type = ParseCommand(command);
			DoCommand(car, type, command);
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
} 
//Add help
//Set speed -1
// Отрбатывает когла сбрасывается скорость на нейтрале
//Наследование, агрегация, композиция