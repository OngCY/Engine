#pragma once

namespace MyTypes
{
	typedef unsigned long EventId;
	typedef unsigned int ActorId;
	typedef unsigned int ComponentId;
	typedef const char* ComponentName;
	typedef unsigned int TranslateType;
}

enum ACTORID :unsigned int
{
	PLAYER,
	BULLET
};

enum COMPONENTS:unsigned int
{
	PICKUP_HEALTH,
	TRANSLATE,
	HEALTH_LIFE,
	BALLISTICS
};

enum TRANSLATION :unsigned int
{
	STATIONARY,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};