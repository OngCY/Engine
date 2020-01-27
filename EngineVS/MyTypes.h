#pragma once

namespace MyTypes
{
	typedef unsigned long EventId;
	typedef unsigned int ActorId;
	typedef unsigned int ComponentId;
	typedef const char* ComponentName;
	typedef unsigned int TranslateType;
}

enum COMPONENTS:unsigned int
{
	PICKUP_HEALTH,
	HEALTH_LIFE
};

enum TRANSLATION :unsigned int
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};