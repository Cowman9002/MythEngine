#include "CoreEngine.h"

#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 560

#include <iostream>

//TODO: clean up scene and resource loading cpp file
//TODO: gui rendering: text, panels
//TODO: editor
//TODO: clean up script engine

int main(int argc, char *argv[])
{
    myth::CoreEngine coreEngine;
    coreEngine.initialize(WINDOW_WIDTH, WINDOW_HEIGHT);

    coreEngine.start();

    coreEngine.terminate();

    return 0;
}
