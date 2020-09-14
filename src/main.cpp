#include "CoreEngine.h"

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 520

#include "ScriptEngine.h"

//TODO: continue removing stuff from C++ code
//TODO: clean-up scene loading
//TODO: material control in scripts

int main(int argc, char *argv[])
{
    myth::CoreEngine coreEngine;
    coreEngine.initialize(WINDOW_WIDTH, WINDOW_HEIGHT);

    coreEngine.start();

    coreEngine.terminate();

    return 0;
}
