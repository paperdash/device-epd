#include "Icons.h"

#define NELEMENTS(array) (sizeof(array) / sizeof((array)[0]))

const unsigned char *getIconById(const char id[], int size)
{
	for (int i = 0; i < NELEMENTS(IconIds); i++)
	{
		if (strcmp(IconIds[i].id, id) == 0)
		{
			switch (size)
			{
			case 64:
				return IconIds[i].icon64;
			case 256:
				return IconIds[i].icon256;
			default:
				return NULL;
			}
		}
	}

	return NULL;
}