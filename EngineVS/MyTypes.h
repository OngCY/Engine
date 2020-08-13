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
	PLAYER
};

enum COMPONENTS:unsigned int
{
	PICKUP_HEALTH,
	TRANSLATE,
	HEALTH_LIFE
};

enum TRANSLATION :const unsigned int
{
	STATIONARY,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};