#include "Grid.h"
#include "MySound.h"
#include "Physics.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <limits>
#include <random>

class Game {
    public:
        Game(std::string mode, bool demo, int demoTime, int autoSpawnRate, int forceSpawnMin, int forceSpawnMax);
        ~Game();

        void displayMusicTime();
        void getPercentSand(); 
        void displayDifficultyLevel();
        void spawnSandParticles();
        void handleMouseInput();
        bool handleKeyboardInput();

    private:
        int numberOfSandParticles;
        int difficultyLevel;

        MySound sound;
        Grid grid;
        Physics physics;

        std::string MODE;
        bool DEMO;
        int DEMO_TIME;
        int AUTO_SPAWN_RATE;
        int FORCE_SPAWN_MIN;
        int FORCE_SPAWN_MAX;

        int lastAutoExecution;
        bool autoModeActive = false;
        int elapsedTime;

        const int NUM_AUTO_CLICKS = 3;

        void startGame();
};