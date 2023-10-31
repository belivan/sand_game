#include "Game.h"
#include "yssimplesound.h"
#include "fssimplewindow.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <limits>
#include <random>

std::string MODE = "AUTO"; // change to "AUTO" or "MANUAL"
bool DEMO = true; // change to "TRUE" or "FALSE"

int DEMO_TIME = 145; // in seconds
int AUTO_SPAWN_RATE = 3; // in seconds
int FORCE_SPAWN_MIN = 30; // in seconds
int FORCE_SPAWN_MAX = 32; // in seconds

int main(void)
{
    Game game(MODE, DEMO, DEMO_TIME, AUTO_SPAWN_RATE, FORCE_SPAWN_MIN, FORCE_SPAWN_MAX);

    return 0;
}