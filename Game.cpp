#include "Game.h"

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

void Game::displayInstructions() 
{
    int wid, hei;
    FsGetWindowSize(wid, hei);

    // Define the instructions
    std::vector<std::string> instructions = {
        "SAND SURVIVAL",
        "INSTRUCTIONS:",
        "1. Sand will fill up.",
        "2. Make sure to remove it.",
        "3. Don't catch the wrong sound 'wave'!",
        "4. Enjoy :)"
    };

    // Set the color and position of the text
    glColor4ub(255, 255, 255,0); 

    // Display each instruction
    for (int i = 0; i < instructions.size(); i++) {
        // Calculate the x-coordinate for centering the text
        int textWidth = (i == 0) ? instructions[i].size() * 20 : instructions[i].size() * 16;
        int x = (wid - textWidth) / 2;

        glRasterPos2d(x, 100 + i * 30);
        if (i == 0) {
            // Use a larger font for the title
            YsGlDrawFontBitmap20x32(instructions[i].c_str());
        } else {
            YsGlDrawFontBitmap16x24(instructions[i].c_str());
        }
    }
    glFlush();
}

void Game::loadBackgroundImage()
{
    FsChangeToProgramDir();

    YsRawPngDecoder png;
    if(!YSOK==png.Decode("background.png"))
    {
       printf("Background Read Error!\n");
    }

	texId=0;

    glGenTextures(1,&texId);  // Reserve one texture identifier
    glBindTexture(GL_TEXTURE_2D,texId);  // Making the texture identifier current (or bring it to the deck)

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    glTexImage2D
        (GL_TEXTURE_2D,
         0,    // Level of detail
         GL_RGBA,
         png.wid,
         png.hei,
         0,    // Border width, but not supported and needs to be 0.
         GL_RGBA,
         GL_UNSIGNED_BYTE,
         png.rgba);
}

void Game::displayBackgroundImage() 
{
    
    int wid,hei;
    FsGetWindowSize(wid,hei);

    glEnable(GL_TEXTURE);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texId);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);


    glColor4f(1,1,1,1);
    glBegin(GL_TRIANGLE_FAN);

    glTexCoord2f(0,0);
    glVertex2i(0,0);

    glTexCoord2f(1,0);
    glVertex2i(wid,0);

    glTexCoord2f(1,1);
    glVertex2i(wid,hei);
    
    glTexCoord2f(0,1);
    glVertex2i(0,hei);

    glEnd();


    glDisable(GL_TEXTURE);
}

void Game::startGame() {

    FsPassedTime();

    FsOpenWindow(0,0,1024,768,1);

    int wid,hei;
    FsGetWindowSize(wid,hei);
    glViewport(0,0,wid,hei);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    loadBackgroundImage();

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

        displayBackgroundImage();
        glDisable(GL_TEXTURE_2D);

        //grid.background();
        sound->drawSoundWave();
        sound->drawSoundWaveBase();

        // Display instructions for the first 9 seconds
        if (elapsedTime < 9000) {
            displayInstructions();
        } 
        else {
            // Start dropping sand and using automatedUser after 9 seconds
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
            
            grid.render();
        }

        //glClear(GL_COLOR_BUFFER_BIT);
        displayStats();

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
            LEVEL = std::round(rI); //rounded

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

void Game::displayStats()
{
    displayMusicTime();
    displayDifficultyLevel();
    displayPercentage();
}

void Game::displayMusicTime() {
    //glClear(GL_COLOR_BUFFER_BIT);
    glColor4ub(255, 255, 255, 0); 
    glRasterPos2d(50,50);

    int totalSeconds = static_cast<int>(sound->getMusicTime());
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    std::ostringstream musicTimeStr;
    musicTimeStr << std::setw(2) << std::setfill('0') << minutes;
    musicTimeStr << ":";
    musicTimeStr << std::setw(2) << std::setfill('0') << seconds;

    YsGlDrawFontBitmap16x24(musicTimeStr.str().c_str());
    glFlush();
}

int Game::getPercentSand() {
    int percentageSand = static_cast<int>((double)(grid.getParticleCount())/(double)(grid.getMaxParticles())*100);
    // std::cout << "PERCENT FULL: " << percentageSand << std::endl;
    return percentageSand;
}

void Game::displayDifficultyLevel() {
    int wid, hei;
    FsGetWindowSize(wid,hei);

    glColor4ub(255, 255, 255, 0); 

    std::string levelStr = "INTENSITY: " + std::to_string(LEVEL);
    int textWidth = 20 * levelStr.length();
    int x = (wid - textWidth) / 2;

    glRasterPos2d(x, 50);

    YsGlDrawFontBitmap20x32(levelStr.c_str());
    glFlush();
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

void Game::displayPercentage() 
{
    int wid, hei;
    FsGetWindowSize(wid, hei);

    // Calculate the dimensions of the bar
    int barWidth = 50;
    int barHeight = static_cast<int>(getPercentSand() * 150 / 100);
    int barX = wid - barWidth - 50;

    // Draw the bar
    glColor4f(1.0, 1.0, 1.0, 0.5);
    glBegin(GL_QUADS);
    glVertex2i(barX, 50 + 150);
    glVertex2i(wid-50, 50 + 150);
    glVertex2i(wid-50, 50 + 150 - barHeight);
    glVertex2i(barX, 50 + 150 - barHeight);
    glEnd();

    // Draw the percentage text
    glColor4ub(255, 255, 255, 0); 
    glRasterPos2d(wid - 350 / 2, 50);

    std::string percentStr = "FILL: " + std::to_string(static_cast<int>(getPercentSand())) + "%";
    YsGlDrawFontBitmap16x24(percentStr.c_str());
    glFlush();
}