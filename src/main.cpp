#include "CoreEngine.h"

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 520

#include "ScriptEngine.h"

int main(int argc, char *argv[])
{
    myth::CoreEngine coreEngine;
    coreEngine.initialize(WINDOW_WIDTH, WINDOW_HEIGHT);

    coreEngine.start();

    coreEngine.terminate();

    return 0;
}
