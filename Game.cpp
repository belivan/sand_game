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
#include <string>
//#include "ysglfontdata.h"

Game::Game(std::string mode, bool demo, int demoTime, int autoSpawnRate, int forceSpawnMin, int forceSpawnMax) {
    MODE = mode;
    DEMO = demo;
    DEMO_TIME = demoTime;
    AUTO_SPAWN_RATE = autoSpawnRate;
    FORCE_SPAWN_MIN = forceSpawnMin;
    FORCE_SPAWN_MAX = forceSpawnMax;
    
    startGame();
}

Game::~Game() {
    // cleanup code here
}

bool Game::handleKeyboardInput() {
    FsPollDevice();
        auto key=FsInkey();
        if(FSKEY_ESC==key)
        {
            return true;
        }
        if(FSKEY_M==key)
        {
            MODE = "MANUAL";
        }
        if(FSKEY_A==key)
        {
            MODE = "AUTO";
        }

    return false;
}

void Game::startGame() {

    FsPassedTime();

    FsOpenWindow(0,0,1024,768,1);
    int wid,hei;
    FsGetWindowSize(wid,hei);
    glViewport(0,0,wid,hei);
  
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    srand(time(NULL));

    //MySound sound;
    //Grid grid;

    auto elapsedTime = 0;
    while(elapsedTime < DEMO_TIME * 1000)
    {
        elapsedTime += FsPassedTime();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, 1024,768, 0, -1, 1);

        if(handleKeyboardInput())
        {
            break; //end on ESC
        }

        handleMouseInput();

        grid.update(sound);
        grid.background();
        sound.drawSoundWave();
        sound.drawSoundWaveBase();
        grid.render();

        FsSwapBuffers();
        FsSleep(30);
    }

}

void Game::displayMusicTime() {
    glColor3ub(255,255,255);
    glRasterPos2d(50,50);

    std::string musicTimeStr = "Music time: " + std::to_string(sound.getMusicTime());
    //YsGlDrawFontBitmap8x12(musicTimeStr.c_str()); //not available yet
}

void Game::getPercentSand() {
    int percentageSand = grid.getParticleCount()/grid.getMaxParticles()*100;
    //std::cout << "Number of sand particles: " << numberOfSandParticles << std::endl;
}

void Game::displayDifficultyLevel() {
    std::cout << "Difficulty level: " << difficultyLevel << std::endl;
}

void Game::handleMouseInput() {
    int wid,hei;
    FsGetWindowSize(wid,hei);

    if (MODE == "AUTO")
        {
            if ((!autoModeActive && (elapsedTime - lastAutoExecution >= AUTO_SPAWN_RATE*1000)) || (elapsedTime >= FORCE_SPAWN_MIN*1000 && elapsedTime <=FORCE_SPAWN_MAX*1000))
            {
                // Transition from inactive to active
                autoModeActive = true;
                lastAutoExecution = elapsedTime;
            }
            else if (autoModeActive && (elapsedTime - lastAutoExecution >= AUTO_SPAWN_RATE*1000) )
            {
                // Transition from active to inactive
                autoModeActive = false;
                lastAutoExecution = elapsedTime;
            }

            if (autoModeActive)
            {
                const int windowWidth = wid;
                const int clickSpacing = wid / NUM_AUTO_CLICKS;

                for (int i = 0; i < NUM_AUTO_CLICKS; ++i)
                {
                    int mx = i * clickSpacing + clickSpacing / 2;
                    int my = hei - hei/5;
                    grid.handleMouseClick(mx, my);
                }
            }
        }

        else if(MODE == "MANUAL" || DEMO == false)
        {
            int lb,mb,rb,mx,my;
            auto evt=FsGetMouseEvent(lb,mb,rb,mx,my);
            if(0 != lb)
            {
                grid.handleMouseClick(mx, my);
            }
        }
}