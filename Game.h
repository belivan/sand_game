#ifndef GAME_H
#define GAME_H

#include "Grid.h"
#include "MySound.h"
#include "Physics.h"

#include "yssimplesound.h"
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "yspng.h"
#include "yspngenc.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <iomanip>

class Game {
    public:
        Game(std::string mode, bool demo, int demoTime);
        ~Game();

        void displayMusicTime();
        int getPercentSand(); 
        void displayDifficultyLevel();
        void handleMouseInput();
        bool handleKeyboardInput();
        void basicSandGenerator();

        void sandGenerator();
        float getAdjustedIntensity();
        void spawnSand(float intensity);

        void automatedUser();

        void displayStats();
        void displayPercentage();
        void displayInstructions();
        void displayBackgroundImage();
        void loadBackgroundImage();

    private:
        int numberOfSandParticles;
        int LEVEL = 0;

        MySound* sound;
        Grid grid;
        Physics physics;
        GLuint texId;

        std::string MODE;
        bool DEMO;
        int DEMO_TIME;
        int AUTO_SPAWN_RATE;
        int FORCE_SPAWN_MIN;
        int FORCE_SPAWN_MAX;

        int lastAutoExecution = 0;
        bool autoModeActive = false;
        int elapsedTime = 0;

        const int NUM_AUTO_CLICKS = 5;

        void startGame();
        void songSelect(std::string song);

        int USER_INTENSITY = 9;
        bool INTENSITY_FLAG = true;

        void reset();
        bool RESET_FLAG = false;
        void pause();
        bool PAUSE_FLAG = false;

        bool USER;
        bool sandGeneratorActive = false;
        double sandGeneratorIntensity = 0.0;

        bool TURN_OFF_VIBRATION = true;
        int TEST_INTENSITY = 0;
        int lastIntensityIncrease = 0;
        int activationStartTime = 0;
        float Kx = 1.0;
        float Ky = 1.0;

        std::string song0 = "demo.wav";
        std::string song1 = "disfigure.wav";
        std::string song2 = "fading.wav";
        std::string song3 = "dawn.wav";
        std::string song4 = "dolce.wav";
};

#endif // GAME_H