#include "faceWeatherIcons.h"

#define NELEMENTS(array) (sizeof(array) / sizeof((array)[0]))

const uint *getIconById(const char id[], int size)
{
	for (int i = 0; i < NELEMENTS(IconIds); i++)
	{
		if (strcmp(IconIds[i].id, id) == 0)
		{
			switch (size)
			{
			case 64:
				return IconIds[i].icon64;
			case 96:
				return IconIds[i].icon96;
			case 128:
				return IconIds[i].icon128;
			case 192:
				return IconIds[i].icon192;
			default:
				return NULL;
			}
		}
	}

	return NULL;
}