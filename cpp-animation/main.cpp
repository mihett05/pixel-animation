

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "App.h"

int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }
    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        printf("IMG Error: %s\n", IMG_GetError());
        return -1;
    }
    App* app = new App();
    app->run();
    return 0;
}