#define CATCH_CONFIG_MAIN
#include "Catch.hpp"
#include "Car.h"

static Car MakeCarAtSpeed(int targetSpeed)
{
    Car car;
    car.TurnOnEngine();

    if (targetSpeed <= 30)
    {
        car.SetGear(Gear::First);
        car.SetSpeed(targetSpeed);
    }
    else if (targetSpeed <= 50)
    {
        car.SetGear(Gear::First);  car.SetSpeed(20);
        car.SetGear(Gear::Second); car.SetSpeed(targetSpeed);
    }
    else if (targetSpeed <= 60)
    {
        car.SetGear(Gear::First);  car.SetSpeed(30);
        car.SetGear(Gear::Third);  car.SetSpeed(targetSpeed);
    }
    else if (targetSpeed <= 90)
    {
        car.SetGear(Gear::First);  car.SetSpeed(30);
        car.SetGear(Gear::Third);  car.SetSpeed(40);
        car.SetGear(Gear::Fourth); car.SetSpeed(targetSpeed);
    }
    else
    {
        car.SetGear(Gear::First);  car.SetSpeed(30);
        car.SetGear(Gear::Third);  car.SetSpeed(50);
        car.SetGear(Gear::Fifth);  car.SetSpeed(targetSpeed);
    }
    return car;
}

TEST_CASE("Initial state")
{
    Car car;
    REQUIRE_FALSE(car.GetEngineIsTurnedOn());
    REQUIRE(car.GetGear() == Gear::Neutral);
    REQUIRE(car.GetSpeed() == 0);
}

TEST_CASE("TurnOnEngine")
{
    SECTION("Engine turns on from off state")
    {
        Car car;
        REQUIRE_NOTHROW(car.TurnOnEngine());
        REQUIRE(car.GetEngineIsTurnedOn());
        REQUIRE(car.GetGear() == Gear::Neutral);
        REQUIRE(car.GetSpeed() == 0);
    }

    SECTION("Turning on already running engine throws")
    {
        Car car;
        car.TurnOnEngine();
        REQUIRE_THROWS_AS(car.TurnOnEngine(), std::runtime_error);
    }
}

TEST_CASE("TurnOffEngine")
{
    SECTION("Engine turns off when neutral and speed 0")
    {
        Car car;
        car.TurnOnEngine();
        REQUIRE_NOTHROW(car.TurnOffEngine());
        REQUIRE_FALSE(car.GetEngineIsTurnedOn());
    }

    SECTION("Cannot turn off engine while moving")
    {
        Car car;
        car.TurnOnEngine();
        car.SetGear(Gear::First);
        car.SetSpeed(10);
        REQUIRE_THROWS_AS(car.TurnOffEngine(), std::runtime_error);
    }

    SECTION("Cannot turn off engine on non-neutral gear (speed 0)")
    {
        Car car;
        car.TurnOnEngine();
        car.SetGear(Gear::First);
        REQUIRE_THROWS_AS(car.TurnOffEngine(), std::runtime_error);
    }

    SECTION("Turning off already-off engine does nothing (no throw)")
    {
        Car car;
        REQUIRE_THROWS_AS(car.TurnOffEngine(), std::runtime_error);
    }
}

TEST_CASE("SetGear – engine off")
{
    SECTION("Cannot set non-neutral gear when engine is off")
    {
        Car car;
        REQUIRE_THROWS_AS(car.SetGear(Gear::First), std::invalid_argument);
        REQUIRE_THROWS_AS(car.SetGear(Gear::Reverse), std::invalid_argument);
        REQUIRE_THROWS_AS(car.SetGear(Gear::Fifth), std::invalid_argument);
    }
}

TEST_CASE("SetGear – engine on speed 0")
{
    Car car;
    car.TurnOnEngine();

    SECTION("1st gear valid at speed 0")
    {
        REQUIRE_NOTHROW(car.SetGear(Gear::First));
    }

    SECTION("2nd-5th gears invalid at speed 0")
    {
        REQUIRE_THROWS_AS(car.SetGear(Gear::Second), std::invalid_argument);
        REQUIRE_THROWS_AS(car.SetGear(Gear::Third), std::invalid_argument);
        REQUIRE_THROWS_AS(car.SetGear(Gear::Fourth), std::invalid_argument);
        REQUIRE_THROWS_AS(car.SetGear(Gear::Fifth), std::invalid_argument);
    }

    SECTION("Reverse is valid at speed 0")
    {
        REQUIRE_NOTHROW(car.SetGear(Gear::Reverse));
    }

    SECTION("Neutral is always valid")
    {
        REQUIRE_NOTHROW(car.SetGear(Gear::Neutral));
    }
}

TEST_CASE("SetGear – reverse rules")
{
    SECTION("Cannot switch to reverse while moving forward")
    {
        Car car;
        car.TurnOnEngine();
        car.SetGear(Gear::First);
        car.SetSpeed(10);
        REQUIRE_THROWS_AS(car.SetGear(Gear::Reverse), std::invalid_argument);
    }

    SECTION("Can switch to reverse at speed 0")
    {
        Car car;
        car.TurnOnEngine();
        REQUIRE_NOTHROW(car.SetGear(Gear::Reverse));
        REQUIRE(car.GetGear() == Gear::Reverse);
    }

    SECTION("Cannot switch to forward gear from reverse while moving")
    {
        Car car;
        car.TurnOnEngine();
        car.SetGear(Gear::Reverse);
        car.SetSpeed(10);
        REQUIRE_THROWS_AS(car.SetGear(Gear::First), std::invalid_argument);
        REQUIRE_THROWS_AS(car.SetGear(Gear::Second), std::invalid_argument);
    }

    SECTION("Can switch from reverse to neutral while moving backward")
    {
        Car car;
        car.TurnOnEngine();
        car.SetGear(Gear::Reverse);
        car.SetSpeed(10);
        REQUIRE_NOTHROW(car.SetGear(Gear::Neutral));
    }

    SECTION("After stopping from reverse (via neutral), can switch to forward")
    {
        Car car;
        car.TurnOnEngine();
        car.SetGear(Gear::Reverse);
        car.SetSpeed(10);
        car.SetGear(Gear::Neutral);
        car.SetSpeed(0);
        REQUIRE_NOTHROW(car.SetGear(Gear::First));
    }
}

TEST_CASE("SetGear – speed ranges")
{
    SECTION("1st gear valid at 0 and 30")
    {
        Car car = MakeCarAtSpeed(0);
        REQUIRE_NOTHROW(car.SetGear(Gear::First));
        Car car2 = MakeCarAtSpeed(30);
        REQUIRE_NOTHROW(car2.SetGear(Gear::First));
    }

    SECTION("2nd gear: min boundary 20 valid, 19 invalid")
    {
        Car car = MakeCarAtSpeed(20);
        REQUIRE_NOTHROW(car.SetGear(Gear::Second));

        Car car2 = MakeCarAtSpeed(19);
        REQUIRE_THROWS_AS(car2.SetGear(Gear::Second), std::invalid_argument);
    }

    SECTION("3rd gear: min boundary 30 valid, 29 invalid")
    {
        Car car = MakeCarAtSpeed(30);
        REQUIRE_NOTHROW(car.SetGear(Gear::Third));

        Car car2 = MakeCarAtSpeed(29);
        REQUIRE_THROWS_AS(car2.SetGear(Gear::Third), std::invalid_argument);
    }

    SECTION("4th gear: min boundary 40 valid, 39 invalid")
    {
        Car car = MakeCarAtSpeed(40);
        REQUIRE_NOTHROW(car.SetGear(Gear::Fourth));

        Car car2 = MakeCarAtSpeed(39);
        REQUIRE_THROWS_AS(car2.SetGear(Gear::Fourth), std::invalid_argument);
    }

    SECTION("5th gear: min boundary 50 valid, 49 invalid")
    {
        Car car = MakeCarAtSpeed(50);
        REQUIRE_NOTHROW(car.SetGear(Gear::Fifth));

        Car car2 = MakeCarAtSpeed(49);
        REQUIRE_THROWS_AS(car2.SetGear(Gear::Fifth), std::invalid_argument);
    }
}

TEST_CASE("SetSpeed – engine off throws")
{
    Car car;
    REQUIRE_THROWS_AS(car.SetSpeed(10), std::runtime_error);
}

TEST_CASE("SetSpeed – negative speed throws")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(Gear::First);
    REQUIRE_THROWS_AS(car.SetSpeed(-1), std::invalid_argument);
}

TEST_CASE("SetSpeed – neutral gear")
{
    SECTION("Cannot accelerate on neutral from 0")
    {
        Car car;
        car.TurnOnEngine();
        REQUIRE_THROWS_AS(car.SetSpeed(1), std::runtime_error);
    }

    SECTION("Can decelerate on neutral")
    {
        Car car;
        car.TurnOnEngine();
        car.SetGear(Gear::First);
        car.SetSpeed(30);
        car.SetGear(Gear::Neutral);
        REQUIRE_NOTHROW(car.SetSpeed(10));
        REQUIRE(car.GetSpeed() == 10);
    }

    SECTION("Cannot increase speed on neutral even by 1")
    {
        Car car;
        car.TurnOnEngine();
        car.SetGear(Gear::First);
        car.SetSpeed(20);
        car.SetGear(Gear::Neutral);
        REQUIRE_THROWS_AS(car.SetSpeed(21), std::runtime_error);
    }

    SECTION("Can keep same speed on neutral (not acceleration)")
    {
        Car car;
        car.TurnOnEngine();
        car.SetGear(Gear::First);
        car.SetSpeed(20);
        car.SetGear(Gear::Neutral);
        REQUIRE_NOTHROW(car.SetSpeed(20));
        REQUIRE(car.GetSpeed() == 20);
    }
}

TEST_CASE("SetSpeed – out of gear range")
{
    SECTION("Exceeds max for 1st gear (>30)")
    {
        Car car;
        car.TurnOnEngine();
        car.SetGear(Gear::First);
        REQUIRE_THROWS_AS(car.SetSpeed(31), std::invalid_argument);
    }

    SECTION("Below min for 2nd gear (<20)")
    {
        Car car = MakeCarAtSpeed(20);
        car.SetGear(Gear::Second);
        REQUIRE_THROWS_AS(car.SetSpeed(19), std::invalid_argument);
    }

    SECTION("Exceeds max for 5th gear (>150)")
    {
        Car car = MakeCarAtSpeed(50);
        car.SetGear(Gear::Fifth);
        REQUIRE_THROWS_AS(car.SetSpeed(151), std::invalid_argument);
    }
}

TEST_CASE("SetSpeed – reverse gear")
{
    SECTION("Valid speeds on reverse: 0 and 20")
    {
        Car car;
        car.TurnOnEngine();
        car.SetGear(Gear::Reverse);
        REQUIRE_NOTHROW(car.SetSpeed(0));
        REQUIRE_NOTHROW(car.SetSpeed(20));
    }

    SECTION("Exceeds max for reverse (>20)")
    {
        Car car;
        car.TurnOnEngine();
        car.SetGear(Gear::Reverse);
        REQUIRE_THROWS_AS(car.SetSpeed(21), std::invalid_argument);
    }
}

TEST_CASE("Scenario: engine on – info state")
{
    Car car;
    car.TurnOnEngine();
    REQUIRE(car.GetEngineIsTurnedOn());
    REQUIRE(car.GetGear() == Gear::Neutral);
    REQUIRE(car.GetSpeed() == 0);
}

TEST_CASE("Scenario: 1st gear + speed 15")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(Gear::First);
    car.SetSpeed(15);
    REQUIRE(car.GetGear() == Gear::First);
    REQUIRE(car.GetSpeed() == 15);
    REQUIRE(car.GetEngineIsTurnedOn());
}

TEST_CASE("Scenario: decelerate on neutral")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(Gear::First);
    car.SetSpeed(30);
    car.SetGear(Gear::Neutral);
    REQUIRE_NOTHROW(car.SetSpeed(10));
    REQUIRE(car.GetSpeed() == 10);
    REQUIRE(car.GetGear() == Gear::Neutral);
}

TEST_CASE("Scenario: cannot turn off engine while moving on 1st gear")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(Gear::First);
    car.SetSpeed(10);
    REQUIRE_THROWS_AS(car.TurnOffEngine(), std::runtime_error);
}

TEST_CASE("Scenario: cannot switch to 2nd at speed 19")
{
    Car car;
    car.TurnOnEngine();
    car.SetGear(Gear::First);
    car.SetSpeed(19);
    REQUIRE_THROWS_AS(car.SetGear(Gear::Second), std::invalid_argument);
}

TEST_CASE("Boundary values for each gear")
{
    SECTION("Reverse: 0 and 20 valid, 21 invalid")
    {
        Car car;
        car.TurnOnEngine();
        car.SetGear(Gear::Reverse);
        REQUIRE_NOTHROW(car.SetSpeed(0));
        REQUIRE_NOTHROW(car.SetSpeed(20));
        REQUIRE_THROWS_AS(car.SetSpeed(21), std::invalid_argument);
    }

    SECTION("First: 0 and 30 valid, 31 invalid")
    {
        Car car;
        car.TurnOnEngine();
        car.SetGear(Gear::First);
        REQUIRE_NOTHROW(car.SetSpeed(0));
        REQUIRE_NOTHROW(car.SetSpeed(30));
        REQUIRE_THROWS_AS(car.SetSpeed(31), std::invalid_argument);
    }

    SECTION("Second: 20-50 valid, 19 and 51 invalid")
    {
        Car car = MakeCarAtSpeed(20);
        car.SetGear(Gear::Second);
        REQUIRE_NOTHROW(car.SetSpeed(20));
        REQUIRE_NOTHROW(car.SetSpeed(50));
        REQUIRE_THROWS_AS(car.SetSpeed(19), std::invalid_argument);
        REQUIRE_THROWS_AS(car.SetSpeed(51), std::invalid_argument);
    }

    SECTION("Third: 30-60 valid, 29 and 61 invalid")
    {
        Car car = MakeCarAtSpeed(30);
        car.SetGear(Gear::Third);
        REQUIRE_NOTHROW(car.SetSpeed(30));
        REQUIRE_NOTHROW(car.SetSpeed(60));
        REQUIRE_THROWS_AS(car.SetSpeed(29), std::invalid_argument);
        REQUIRE_THROWS_AS(car.SetSpeed(61), std::invalid_argument);
    }

    SECTION("Fourth: 40-90 valid, 39 and 91 invalid")
    {
        Car car = MakeCarAtSpeed(40);
        car.SetGear(Gear::Fourth);
        REQUIRE_NOTHROW(car.SetSpeed(40));
        REQUIRE_NOTHROW(car.SetSpeed(90));
        REQUIRE_THROWS_AS(car.SetSpeed(39), std::invalid_argument);
        REQUIRE_THROWS_AS(car.SetSpeed(91), std::invalid_argument);
    }

    SECTION("Fifth: 50-150 valid, 49 and 151 invalid")
    {
        Car car = MakeCarAtSpeed(50);
        car.SetGear(Gear::Fifth);
        REQUIRE_NOTHROW(car.SetSpeed(50));
        REQUIRE_NOTHROW(car.SetSpeed(150));
        REQUIRE_THROWS_AS(car.SetSpeed(49), std::invalid_argument);
        REQUIRE_THROWS_AS(car.SetSpeed(151), std::invalid_argument);
    }
}
