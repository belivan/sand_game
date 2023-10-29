#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "fssimplewindow.h"
#include <iostream>
#include "yssimplesound.h"
#include <limits>
#include <random>

std::string MODE = "AUTO"; // change to "AUTO" or "MANUAL"

class MySound{
    private:
        YsSoundPlayer player;
        YsSoundPlayer::SoundData wav;
        std::string fileName;

        //misc variables
        const double MAX_DISTANCE = 200.0;
        const double MAX_Y = 32767.0;

        void loadWavFile();
        void playWavFile();

    public:
        MySound();
        int getWavCurrentX();
        int getWavCurrentY();
        void drawSoundWave();
};

MySound::MySound()
{
    loadWavFile();
    playWavFile();
}

void MySound::drawSoundWave() //would be nice if worked
{
    int wid,hei;
    FsGetWindowSize(wid,hei);
    int axis = hei / 2;

    auto currentTime = player.GetCurrentPosition(wav);
    auto currentPos = wav.SecToNumSample(currentTime);
    int x = static_cast<int>((currentPos / static_cast<double>(wav.GetNumSample() / wav.GetNumChannel())) * wid);
    int y = wav.GetSignedValue16(0, currentPos);
    y = axis - static_cast<int>(y * (axis / 2.0) / 32767.0);

    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex2i(x, 0);
    glVertex2i(x, hei);
    glEnd();
    
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);

    for(long long int x = 0; x < wid; ++x)
    {
        long long int ptr = x * wav.GetNumSamplePerChannel() / wid;
        int y = wav.GetSignedValue16(0, ptr);
        y = axis - static_cast<int>(y * (axis / 2.0) / 32767.0);
        glVertex2i(x, y);
    }
    glEnd();
}

void MySound::loadWavFile()
{
    FsChangeToProgramDir();
    std::cout << "Enter Wav File Name:";
    std::getline(std::cin,fileName);

    while(YSOK!=wav.LoadWav(fileName.c_str()))
    {
        std::cout << "Failed to read %s\n";
        std::cout << "Enter Wav File Name:";
        std::getline(std::cin,fileName);
    }
}

void MySound::playWavFile()
{
    player.Start();
    player.PlayBackground(wav);

    int ptr0=0;
    int range=wav.GetNumSamplePerChannel();

    // for(;;)
    // {
    //     FsPollDevice();
    //     player.KeepPlaying();

    //     auto key=FsInkey();
    //     if(FSKEY_ESC==key)
    //     {
    //         break;
    //     }
    // }
}

int MySound::getWavCurrentX()
{
    int wid,hei;
    FsGetWindowSize(wid,hei);

    auto currentTime=player.GetCurrentPosition(wav);
    auto currentPos=wav.SecToNumSample(currentTime);

    int x = static_cast<int>((currentPos/static_cast<double>(wav.GetNumSample()/wav.GetNumChannel()))*wid);

    return x;
}

int MySound::getWavCurrentY()
{
    int wid, hei;
    FsGetWindowSize(wid, hei);
    int axis = hei / 2;

    auto currentTime = player.GetCurrentPosition(wav);
    auto currentPos = wav.SecToNumSample(currentTime);

    int y = wav.GetSignedValue16(0, currentPos);
    y = axis - static_cast<int>(y * (axis / 2.0) / MAX_DISTANCE);

    return y;
}



class Particle {
    public:
        int r, g, b; //colors
        float vy; //down velocity
        float vx;

        Particle() : r(0), g(0), b(0), vy(0.0f), vx(0.0f) {} //default constructor

        Particle(int red, int green, int blue, bool setFlag)
        : r(red), g(green), b(blue), vy(0.0f), vx(0.0f) {}

        void turnSand();
        void turnSet();

        //drawing itself
        void drawParticle(int x, int y, int PIXELS_PER_GRID_CELL, int GRID_HEIGHT);
};

void Particle::turnSand()
{
    r = 255;
    g = 255;
    b = 0;
}

void Particle::drawParticle(int x, int y, int PIXELS_PER_GRID_CELL, int GRID_HEIGHT) {
    //shade effect
    float gradientFactor = 1.0f - static_cast<float>(y) / GRID_HEIGHT;
    int red = static_cast<int>(255 * (0.7 + 0.3 * gradientFactor));
    int green = static_cast<int>(150 * (0.7 + 0.3 * gradientFactor));
    int blue = 0;
    
    glColor3ub(red, green, blue);
    
    int left = x * PIXELS_PER_GRID_CELL;
    int right = (x + 1) * PIXELS_PER_GRID_CELL;
    int top = y * PIXELS_PER_GRID_CELL;
    int bottom = (y + 1) * PIXELS_PER_GRID_CELL;

    glBegin(GL_QUADS);
    glVertex2i(left, bottom);
    glVertex2i(right, bottom);
    glVertex2i(right, top);
    glVertex2i(left, top);
    glEnd();
}


class Grid {
    private:
        int PARTICLE_COUNT;
        int MAX_PARTICLES;
        const int MAX_PARTICLES_FACTOR = 10; // 1/n of grid size
        int GRID_WIDTH;
        int GRID_HEIGHT;
        const float VX_DAMP_FACTOR = 0.9; //edit if needed
        Particle* grid;
        const float gravity = 9.81f;  // gravity constant
        const float timeConstant = 0.3f; //time constant
        const int PIXELS_PER_GRID_CELL = 4;
        const int BLOB_SIZE = 10;

        //sound variables
        const double MAX_DISTANCE = 200.0;
        const double MAX_Y = 32767.0;
        const double K_FORCE = 3; //edit if needed

        //activity variables
        bool activityFlag;
        double lastActivityTime;
        const double activityTimeout = 0.9; //IMPORTANT


    public:
        Grid();
        ~Grid();
        void render();
        void handleMouseClick(int mx, int my);
        void createBlob(int X, int Y);
        void removeExcessParticles();
        void update(MySound& sound);

        //functions for particle physics in the grid
        bool isWithinBounds(int x, int y);
        bool moveParticle(int x, int y, int X_NEXT, int Y_NEXT);
        bool handleLateralMovement(int x, int y, int Y_NEXT);
        int getNextMove(int x, int y);

        //handling activity
        bool isActivityTimeoutExpired(double currentTime) {
        return currentTime - lastActivityTime > activityTimeout;
        }

        const int NUM_CLICKS = 3;
};

bool Grid::moveParticle(int x, int y, int X_NEXT, int Y_NEXT) {
    if (isWithinBounds(X_NEXT, Y_NEXT)) {
        Particle& currentParticle = grid[y*GRID_WIDTH + x];
        Particle& newParticle = grid[Y_NEXT*GRID_WIDTH + X_NEXT];
        if (newParticle.r == 0 && newParticle.g == 0 && newParticle.b == 0) {
            newParticle = currentParticle;
            currentParticle = Particle();
            return true; 
        }
    }
    return false;
}

bool Grid::handleLateralMovement(int x, int y, int Y_NEXT) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 1);

    int r = dis(gen) == 0 ? 1 : -1;  // either 1 or -1
    int x_next = x + r;
    if (moveParticle(x, y, x_next, Y_NEXT)) {
        return true;
    }
    
    x_next = x - r;
    return moveParticle(x, y, x_next, Y_NEXT);
}

Grid::Grid() : lastActivityTime(0.0), activityFlag(true), PARTICLE_COUNT(0){ //change if needed and remove
    GRID_WIDTH = 800 / PIXELS_PER_GRID_CELL;
    GRID_HEIGHT = 600 / PIXELS_PER_GRID_CELL;

    grid = new Particle[GRID_WIDTH*GRID_HEIGHT];
    for(int i = 0; i < GRID_WIDTH*GRID_HEIGHT; ++i) {
        grid[i] = Particle();
    }

    MAX_PARTICLES = GRID_WIDTH * GRID_HEIGHT / MAX_PARTICLES_FACTOR;
}

bool Grid::isWithinBounds(int x, int y) {
    return x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT;
}

Grid::~Grid(){
    delete[] grid;
}

int Grid::getNextMove(int x, int y)
{
    int closestEmptyY = y;
    double closestDistance = std::numeric_limits<double>::max();
    for (int y_next = y; y_next < GRID_HEIGHT; ++y_next) {
        Particle targetSpace = grid[y_next*GRID_WIDTH + x];
        if (targetSpace.r == 0 && targetSpace.g == 0 && targetSpace.b == 0) {
            double distance = std::abs(y - y_next);
            if (distance < closestDistance) {
                closestEmptyY = y_next;
                closestDistance = distance;
            }
        }
    }

    return closestEmptyY;
}

void Grid::update(MySound& sound) {

    auto currentX = sound.getWavCurrentX();
    auto currentY = sound.getWavCurrentY();

    for (int y = GRID_HEIGHT - 1; y >= 0; --y) {  
        for (int x = 0; x < GRID_WIDTH; ++x) {
            Particle& currentParticle = grid[y*GRID_WIDTH + x];

            if(currentParticle.r != 0 || currentParticle.g != 0 || currentParticle.b != 0)
            {
                // Calculate distance from particle to current x,y position
                double dx = (int)(currentX / PIXELS_PER_GRID_CELL) - x;
                double dy = (int)(currentY / PIXELS_PER_GRID_CELL) - y;
                double dist = sqrt(dx * dx + dy * dy);

                // Calculate force based on distance and y magnitude of sound wave
                double force = K_FORCE*(1 - dist / MAX_DISTANCE) * fabs(currentY) / MAX_Y;

                // Update velocity based on force
                currentParticle.vx += force * dx / dist;
                currentParticle.vy += force * dy / dist;

                //Apply damping to horizontal velocity and do Euler's for Vx and Vy
                currentParticle.vx *= VX_DAMP_FACTOR;
                int X_NEXT = x + static_cast<int>(currentParticle.vx * timeConstant);

                currentParticle.vy += gravity * timeConstant;
                int Y_NEXT = y + static_cast<int>(currentParticle.vy * timeConstant);

                if(moveParticle(x,y,X_NEXT,Y_NEXT))
                {
                    continue;
                }
                else
                {
                    Y_NEXT = (Y_NEXT < 0) ? 0 : ((Y_NEXT >= GRID_HEIGHT) ? GRID_HEIGHT - 1 : Y_NEXT);
                    X_NEXT = (X_NEXT < 0) ? 0 : ((X_NEXT >= GRID_WIDTH) ? GRID_WIDTH - 1 : X_NEXT);

                    Y_NEXT = getNextMove(x, y);

                    if(!handleLateralMovement(x, y, Y_NEXT))
                    {
                        if(!moveParticle(x, y, X_NEXT, Y_NEXT))
                        {
                            continue;
                        }
                    }
                }
            }
        }
    }
}

void Grid::render() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the color-graded background triangles
    glBegin(GL_TRIANGLES);
    for (int x = 0; x < GRID_WIDTH; ++x) {
        for (int y = 0; y < GRID_HEIGHT; ++y) {
            glColor3ub(x, y, 255);

            int left = x * PIXELS_PER_GRID_CELL;
            int right = (x + 1) * PIXELS_PER_GRID_CELL;
            int top = y * PIXELS_PER_GRID_CELL;
            int bottom = (y + 1) * PIXELS_PER_GRID_CELL;

            glVertex2i(left, bottom);
            glVertex2i(right, bottom);
            glVertex2i(left, top);

            glVertex2i(left, top);
            glVertex2i(right, top);
            glVertex2i(right, bottom);
        }
    }
    glEnd();

    // Draw the particles
    glBegin(GL_QUADS);

    int lastR = -1, lastG = -1, lastB = -1;

    for (int x = 0; x < GRID_WIDTH; ++x) {
        for (int y = GRID_HEIGHT - 1; y >= 0; --y) {
            Particle &p = grid[y*GRID_WIDTH + x];
            if (p.r != 0 || p.g != 0 || p.b != 0) {
                if (p.r != lastR || p.g != lastG || p.b != lastB) {
                    glColor3ub(p.r, p.g, p.b);
                    lastR = p.r;
                    lastG = p.g;
                    lastB = p.b;
                }

               p.drawParticle(x, y, PIXELS_PER_GRID_CELL, GRID_HEIGHT);
            }
        }
    }
    glEnd();
}

void Grid::handleMouseClick(int mx, int my)
{   
    int X = mx / PIXELS_PER_GRID_CELL;
    int Y = my / PIXELS_PER_GRID_CELL;

    createBlob(X, Y);
}

void Grid::createBlob(int X, int Y) {
    for (int i = -BLOB_SIZE; i <= BLOB_SIZE; ++i) {
        for (int j = -BLOB_SIZE; j <= BLOB_SIZE; ++j) {
            int x = X + i;
            int y = Y + j;

            if(isWithinBounds(x,y))
            {
                Particle& p = grid[y*GRID_WIDTH + x];
                if (p.r != 0 || p.g !=0 || p.b != 0) {
                    p = Particle();
                    --PARTICLE_COUNT;
                }
                else {
                    p.turnSand();
                    p.vy = static_cast<float>(rand() % 5 + 5) * -1.0f;
                    p.vx = static_cast<float>(rand() % 5 - 2);
                    ++PARTICLE_COUNT;
                }
            }
        }
    }
    removeExcessParticles();
}

void Grid::removeExcessParticles() {
    while (PARTICLE_COUNT > MAX_PARTICLES) {
        for (int y = GRID_HEIGHT - 1; y > 0; --y) {  // Start from the bottom row, stop before reaching the top
            for (int aboveY = y - 1; aboveY >= 0; --aboveY) {
                for (int x = 0; x < GRID_WIDTH; ++x) {
                    grid[(aboveY + 1) * GRID_WIDTH + x] = grid[aboveY * GRID_WIDTH + x];
                }
            }
            for (int x = 0; x < GRID_WIDTH; ++x) {
                grid[x] = Particle();
            }
            PARTICLE_COUNT -= GRID_WIDTH;
            if (PARTICLE_COUNT <= MAX_PARTICLES) {
                break;
            }
        }
    }
}

int main(void)
{
    FsPassedTime();

    FsOpenWindow(0,0,800,600,1);
    int wid,hei;
    FsGetWindowSize(wid,hei);
    glViewport(0,0,wid,hei);
  
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    srand(time(NULL));

    Grid grid;
    MySound sound;

    int simulationX = 0;  // Starting position for simulation
    const int simulationSpeed = 10;  // Speed at which the simulation progresses, adjust as needed

    for(;;)
    {
        FsPollDevice();
        auto key=FsInkey();
        if(FSKEY_ESC==key)
        {
            break;
        }
        
        if (MODE == "AUTO")
        {
            const int windowWidth = wid;
            const int clickSpacing = wid / grid.NUM_CLICKS;

            for (int i = 0; i < grid.NUM_CLICKS; ++i) {
                int mx = i * clickSpacing + clickSpacing / 2;
                int my = hei - hei/5;
                grid.handleMouseClick(mx, my);
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
        glOrtho(0, 800, 600, 0, -1, 1);

        sound.drawSoundWave();
        grid.update(sound);

        grid.render();

        FsSwapBuffers();
        FsSleep(20);
    }
    return 0;
}