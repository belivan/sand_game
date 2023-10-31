#include "Game.h"

//#include "ysglfontdata.h"

Game::Game(std::string mode, bool demo, int demoTime) {
    MODE = mode;
    DEMO = demo;
    DEMO_TIME = demoTime;

    sound = new MySound(song0);

    //displayMusicTitle(song1);
    
    startGame();
}

Game::~Game() {
    // cleanup code here
    delete sound;
    delete &grid;
    delete &physics;
}

void Game::reset() {
    grid.reset();
    sound->reset();
    RESET_FLAG = true;
}

void Game::pause() {
    PAUSE_FLAG = !PAUSE_FLAG;
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
        if(FSKEY_R == key)
        {
            reset();
        }
        if(FSKEY_1 == key)
        {
            songSelect(song1);
            
            reset();
        }
        if(FSKEY_2 == key)
        {
            songSelect(song2);
            reset();
        }
        if(FSKEY_3 == key)
        {
            songSelect(song3);
            reset();
        }
        if(FSKEY_4 == key)
        {
            songSelect(song4);
            reset();
        }
        // if(FSKEY_SPACE == key)
        // {
        //     pause();
        // }

    return false;
}

void Game::songSelect(std::string song)
{
    if(sound) // check if sound already has a value
    {
        delete sound;
    }
    sound = new MySound(song);
}


void Game::startGame() {

    FsPassedTime();

    FsOpenWindow(0,0,1024,768,1);

    int wid,hei;
    FsGetWindowSize(wid,hei);
    glViewport(0,0,wid,hei);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    srand(time(NULL));

    elapsedTime = 0;

    while(DEMO == false || (DEMO == true && elapsedTime < DEMO_TIME * 1000))
    {

        if(RESET_FLAG == true)
        {
            RESET_FLAG = false;
        }
    
        elapsedTime += FsPassedTime();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, 1024, 768, 0, -1, 1);

        sandGenerator();

        if(MODE == "AUTO")
        {
            automatedUser();
        }

        else if(MODE == "MANUAL")
        {
            handleMouseInput();
        }

        handleMouseInput();

        if(handleKeyboardInput())
        {
            break; //end on ESC
        }

        grid.update(*sound, TURN_OFF_VIBRATION);

        grid.background();

        sound->drawSoundWave();

        sound->drawSoundWaveBase();

        grid.render();

        FsSwapBuffers();

        FsSleep(30);
    }

    FsCloseWindow();
}

void Game::basicSandGenerator()
{
    if (!sandGeneratorActive || elapsedTime - activationStartTime >= 5000)
    {
        // Start the activation phase
        std::cout << "Starting activation phase..." << std::endl;
        sandGeneratorActive = true;
        activationStartTime = elapsedTime;
    }
    else if (sandGeneratorActive && elapsedTime - activationStartTime >= 3000)
    {
        // End the activation phase
        std::cout << "Ending activation phase..." << std::endl;
        sandGeneratorActive = false;
    }

    if (sandGeneratorActive)
    {
        std::cout << "Sand generator is active." << std::endl;
        int wid, hei;
        FsGetWindowSize(wid, hei);

        // Define the height at which you want to spawn sand
        int my = TEST_INTENSITY + 5;

        USER = false;
        INTENSITY_FLAG = true;

        // Spawn sand on the left side
        grid.createSand(TEST_INTENSITY+20, my, TEST_INTENSITY, INTENSITY_FLAG, USER);

        // Spawn sand on the right side
        grid.createSand(wid - TEST_INTENSITY-20, my, TEST_INTENSITY, INTENSITY_FLAG, USER);

        if(elapsedTime - lastIntensityIncrease >= 1000)
        {   
            //Kx++;
            Ky++;
            physics.updatePhysics(Kx, Ky);
        }

        // Only increase intensity if it's been 5 seconds since the last increase
        if(elapsedTime - lastIntensityIncrease >= 5000)
        {
            TEST_INTENSITY++;
            std::cout << "!!!!!!!!!!!!!!!!!!" << std::endl;
            std::cout << "Intensity: " << TEST_INTENSITY << std::endl;
            std::cout << "!!!!!!!!!!!!!!!!!!" << std::endl;
            lastIntensityIncrease = elapsedTime;
        }
    }
}

float Game::getAdjustedIntensity()
{
    float intensity = sound->getSoundWaveMagnitude();

    // Tweak intensity based on time intervals:
    if (elapsedTime % 20000 < 5000 && intensity < 0.2)
    {
        intensity = 0.1;
    }
    else if (elapsedTime % 10000 < 5000 && intensity < 0.3)
    {
        intensity = 0.2;
    }
    else if (elapsedTime % 5000 < 3000 && intensity < 0.4)
    {
        intensity = 0.3;
    }
    else if (elapsedTime % 40000 < 5000)
    {
        intensity = fmax(0.4, intensity);
    }

    return intensity;
}

void Game::spawnSand(float intensity)
{
    int wid, hei;
    FsGetWindowSize(wid, hei);

    const int numClicks = static_cast<int>(intensity * intensity * 100); // Number of points to spawn sand across the screen width
    float sandAmountPerClick = intensity * 10;

    for (int i = 0; i < numClicks; ++i)
    {
        int mx = i * (wid / numClicks);
        int my = sandAmountPerClick + 20;  // Adjust the y-position based on sandAmountPerClick

        USER = false;
        INTENSITY_FLAG = true;
        grid.createSand(mx, my, sandAmountPerClick, INTENSITY_FLAG, USER);
    }
}

void Game::sandGenerator()
{
    // Determine if the sand generator should activate
    // Activate for 3 seconds every 5 seconds
    if (elapsedTime % 5000 < 3000)
    {
        // If we're just starting this phase, determine intensity and store it
        if (!sandGeneratorActive)
        {
            sandGeneratorIntensity = getAdjustedIntensity();

            sandGeneratorActive = true;

            float rI = sandGeneratorIntensity * 10; //scaled
            int roI = std::round(rI); //rounded

            // std::cout << "Sand generator is active!" << std::endl;
            // std::cout << "INTENSITY LEVEL: " << roI << std::endl;
        }

        spawnSand(sandGeneratorIntensity);
    }
    else
    {
        sandGeneratorActive = false; // Reset the flag when not in the active phase
    }
}


void Game::handleMouseInput()
{
    if(MODE == "MANUAL")
    {
        int lb,mb,rb,mx,my;
        auto evt=FsGetMouseEvent(lb,mb,rb,mx,my);
        if(0 != lb)
        {
            USER = true;
            grid.createSand(mx, my, USER_INTENSITY, INTENSITY_FLAG, USER); //in this case USER is subtracting sand (thats the game)
        }
    }
}

void Game::displayMusicTime() {
    glColor3ub(255,255,255);
    glRasterPos2d(50,50);

    std::string musicTimeStr = "Music time: " + std::to_string(sound->getMusicTime());
    //YsGlDrawFontBitmap8x12(musicTimeStr.c_str()); //not available yet
}

int Game::getPercentSand() {
    int percentageSand = static_cast<int>((double)(grid.getParticleCount())/(double)(grid.getMaxParticles())*100);
    // std::cout << "PERCENT FULL: " << percentageSand << std::endl;
    return percentageSand;
}

void Game::displayDifficultyLevel() {
    std::cout << "Difficulty level: " << difficultyLevel << std::endl;
}

void Game::automatedUser()
{
    int percentageSand = getPercentSand();

    // std::cout << "PERCENT FULL: " << percentageSand << std::endl;

    int removalRate;

    // Decide the frequency of sand removal based on the screen fill percentage
    if(percentageSand < 50)
    {
        removalRate = 1;
    }
    else if(percentageSand < 90)
    {
        removalRate = 1;
    }
    else
    {
        removalRate = 1;
    }

    if(elapsedTime % removalRate == 0)
    {
        int wid, hei;
        FsGetWindowSize(wid, hei);

        // Generate random x and y positions
        int mx = rand() % wid;
        int my = rand() % hei;

        USER = true;
        INTENSITY_FLAG = true;
        grid.createSand(mx, my, USER_INTENSITY, INTENSITY_FLAG, USER);
    }
}
