#include "MySound.h"
#include "Particle.h"
#include "Grid.h"
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

int DEMO_TIME = 145; // in seconds

int lastAutoExecution = 0;
bool autoModeActive = false;

int main(void)
{
    FsPassedTime();

    FsOpenWindow(0,0,1024,768,1);
    int wid,hei;
    FsGetWindowSize(wid,hei);
    glViewport(0,0,wid,hei);
  
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    srand(time(NULL));

    Grid grid;
    MySound sound;


    auto elapsedTime = 0;
    while(elapsedTime < DEMO_TIME * 1000)
    {
        elapsedTime += FsPassedTime();

        FsPollDevice();
        auto key=FsInkey();
        if(FSKEY_ESC==key)
        {
            break;
        }
        if(FSKEY_M==key)
        {
            MODE = "MANUAL";
        }
        if(FSKEY_A==key)
        {
            MODE = "AUTO";
        }
        
        if (MODE == "AUTO")
        {
            if ((!autoModeActive && (elapsedTime - lastAutoExecution >= 5000)) || (elapsedTime >= 30000 && elapsedTime <=32000))
            {
                // Transition from inactive to active
                autoModeActive = true;
                lastAutoExecution = elapsedTime;
            }
            else if (autoModeActive && (elapsedTime - lastAutoExecution >= 5000) )
            {
                // Transition from active to inactive
                autoModeActive = false;
                lastAutoExecution = elapsedTime;
            }

            if (autoModeActive)
            {
                const int windowWidth = wid;
                const int clickSpacing = wid / grid.NUM_CLICKS;

                for (int i = 0; i < grid.NUM_CLICKS; ++i)
                {
                    int mx = i * clickSpacing + clickSpacing / 2;
                    int my = hei - hei/5;
                    grid.handleMouseClick(mx, my);
                }
            }
        }

        else if(MODE == "MANUAL")
        {
            int lb,mb,rb,mx,my;
            auto evt=FsGetMouseEvent(lb,mb,rb,mx,my);
            if(0 != lb)
            {
                grid.handleMouseClick(mx, my);
            }
        }

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, 1024,768, 0, -1, 1);

        grid.update(sound);

        grid.background();
        sound.drawSoundWave();
        sound.drawSoundWaveBase();
        grid.render();

        FsSwapBuffers();
        FsSleep(30);
    }
    return 0;
}