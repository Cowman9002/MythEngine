#include "CoreEngine.h"

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 520

#include "ScriptEngine.h"

//TODO: scripting
//TODO: transform integration
//TODO: gameobject integration

//TODO: milestone, have camera track the ball thats currently moving

// https://www.lua.org/manual/5.2/manual.html#4

int main(int argc, char *argv[])
{
    myth::CoreEngine coreEngine;
    coreEngine.initialize(WINDOW_WIDTH, WINDOW_HEIGHT);

    coreEngine.start();

    coreEngine.terminate();

    return 0;
}
