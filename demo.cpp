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

int DEMO_TIME = 131; // in seconds

int main(void)
{
    Game game(MODE, DEMO, DEMO_TIME);
    return 0;
}

//Things to add:
// 1. Add percent full bar
// 2. Add difficulty level
// 3. Add title screen
// 4. Add instructions