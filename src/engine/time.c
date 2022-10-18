#include <SDL2/SDL.h>
#include "engine/time.h"

unsigned long long Time_getTicks()
{
    return SDL_GetTicks64();
}

void Time_delay(unsigned int ms)
{
    SDL_Delay(ms);
}