#ifndef TOOLS_H
#define TOOLS_H

#define EVERY_N_SECONDS(t) for (static uint16_t _lasttime;                                \
								(uint16_t)((uint16_t)millis() - _lasttime) >= (t * 1000); \
								_lasttime += (t * 1000))

#endif