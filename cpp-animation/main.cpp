#include <iostream>
#include <SDL.h>
#include "App.h"

int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }
    App* app = new App();
    app->run();
    return 0;
}