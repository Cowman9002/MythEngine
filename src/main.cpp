#include "CoreEngine.h"

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 520

#include "ScriptEngine.h"

//TODO: move lua script methods to separate files
//TODO: add aabb collider
//TODO: increase functionality of scriptable physics pipeline

int main(int argc, char *argv[])
{
    myth::CoreEngine coreEngine;
    coreEngine.initialize(WINDOW_WIDTH, WINDOW_HEIGHT);

    coreEngine.start();

    coreEngine.terminate();

    return 0;
}
