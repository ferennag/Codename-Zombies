#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "engine/window.h"
#include "engine/game_loop.h"

int main() 
{
    Window_create(1440, 900);
    GameLoop_run();
    Window_close();
    return EXIT_SUCCESS;
}
