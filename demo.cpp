#include <stdio.h>
#include <time.h>
#include <stdlib.h>
//#include <math.h>
#include "fssimplewindow.h"
//#include <vector>
#include <iostream>

class Particle {
    public:
        int r, g, b; //colors
        bool set; //can move or not
        float vy; //down velocity
        float vx;

        Particle() : r(0), g(0), b(0), set(true), vy(0.0f), vx(0.0f) {}

        Particle(int red, int green, int blue, bool setFlag)
        : r(red), g(green), b(blue), set(setFlag), vy(0.0f), vx(0.0f) {}

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
    set = false;
}

void Particle::turnSet()
{
    set = true;
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
        int GRID_WIDTH;
        int GRID_HEIGHT;
        const float VX_DAMP_FACTOR = 0.7;
        Particle* grid;
        const float gravity = 9.81f;  // gravity constant
        const float timeConstant = 0.3f; //time constant
        const int PIXELS_PER_GRID_CELL = 8;
        const int BLOB_SIZE = 3;
        const int maxLateralAttempts = 10;  // Maximum number of lateral movement attempts

        //activity variables
        bool activityFlag;
        double lastActivityTime;
        const double activityTimeout = 0.9; //IMPORTANT

    public:
        Grid();
        void render();
        void handleMouseClick(int mx, int my, double currentTime);
        void createBlob(int X, int Y);

        //functions for particle physics in the grid
        bool isWithinBounds(int x, int y);
        bool moveParticle(int x, int y, int x_next, int y_next);
        bool handleLateralMovement(int x, int y, int y_next);

        //handling activity
        bool isActivityTimeoutExpired(double currentTime) {
        return currentTime - lastActivityTime > activityTimeout;
        }
        void update(double currentTime);


};

bool Grid::moveParticle(int x, int y, int x_next, int y_next) {
    if (isWithinBounds(x_next, y_next)) {
        Particle& currentParticle = grid[y*GRID_WIDTH + x];
        Particle& newParticle = grid[y_next*GRID_WIDTH + x_next];
        if (newParticle.r == 0 && newParticle.g == 0 && newParticle.b == 0) {
            newParticle = currentParticle;
            currentParticle = Particle();
            if (y_next == GRID_HEIGHT - 1) {
                newParticle.turnSet();
                //printf("PARTICLE SET!");
            }
            return true; 
        }
    }
    return false;
}

bool Grid::handleLateralMovement(int x, int y, int y_next) {
    int r = rand() % 2 == 0 ? 1 : -1;  // either 1 or -1
    int x_next = x + r;
    if (moveParticle(x, y, x_next, y_next)) {
        return true;
    }
    
    x_next = x - r;
    return moveParticle(x, y, x_next, y_next);
}

Grid::Grid() : lastActivityTime(0.0), activityFlag(true){ //change if needed and remove
    GRID_WIDTH = 800 / PIXELS_PER_GRID_CELL;
    GRID_HEIGHT = 600 / PIXELS_PER_GRID_CELL;

    grid = new Particle[GRID_WIDTH*GRID_HEIGHT];
    for(int i = 0; i < GRID_WIDTH*GRID_HEIGHT; ++i) {
        grid[i] = Particle();
    }
}

bool Grid::isWithinBounds(int x, int y) {
    return x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT;
}

void Grid::update(double currentTime) {
    for (int y = GRID_HEIGHT - 1; y >= 0; --y) {  
        for (int x = 0; x < GRID_WIDTH; ++x) {
            Particle& currentParticle = grid[y*GRID_WIDTH + x];

            if(currentParticle.r != 0 || currentParticle.g != 0 || currentParticle.b != 0)
            {
                bool movedX = false;
                bool movedY = false;

                if(currentParticle.set) //to prevent cavities
                {
                    int moves[4][2] = {{0, 1}, {1, 1}, {-1, 1}, {0, 0}};  // Possible moves: down, down-right, down-left, no move
                    for (int i = 0; i < 4; ++i) {
                        int x_next = x + moves[i][0];
                        int y_next = y + moves[i][1];
                        if (moveParticle(x, y, x_next, y_next)) {
                            currentParticle.set = false;
                            continue;
                        }
                    }
                }
                else
                {
                    // Apply damping to horizontal velocity and do Euler's for Vx and Vy
                    currentParticle.vx *= VX_DAMP_FACTOR;
                    int x_next = x + static_cast<int>(currentParticle.vx * timeConstant);

                    currentParticle.vy += gravity * timeConstant;
                    int y_next = y + static_cast<int>(currentParticle.vy * timeConstant);

                    if(isWithinBounds(x_next, y_next)) {
                        if(moveParticle(x,y,x_next,y_next))
                        {
                            continue;
                        }
                    }
                    else
                    {
                        y_next = (y_next < 0) ? 0 : ((y_next >= GRID_HEIGHT) ? GRID_HEIGHT - 1 : y_next);
                        x_next = (x_next < 0) ? 0 : ((x_next >= GRID_WIDTH) ? GRID_WIDTH - 1 : x_next);
                    }
                    
                    int lateralAttempts = 0;
                    if(!moveParticle(x, y, x_next, y_next)) {  // Try to move down
                        lateralAttempts = 0;
                        while(!handleLateralMovement(x, y, y_next) && lateralAttempts < maxLateralAttempts) {  // Try to move laterally
                            --y_next;
                            ++lateralAttempts;
                        }
                        if(lateralAttempts>=maxLateralAttempts) {
                                currentParticle.turnSet(); // Set particle if it can't move
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

void Grid::handleMouseClick(int mx, int my, double currentTime)
{   
    //std::cout << "Mouse click at pixel coordinates (" << mx << ", " << my << ")" << std::endl;

    int X = mx / PIXELS_PER_GRID_CELL;
    int Y = my / PIXELS_PER_GRID_CELL;

    //std::cout << "Grid coordinates (" << X << ", " << Y << ")" << std::endl;
    //TIME_SINCE_CLICK = static_cast<double>(elapsedTime);
    createBlob(X, Y);
    lastActivityTime = currentTime;

    
}

void Grid::createBlob(int X, int Y)
{
    for (int i = -BLOB_SIZE; i <= BLOB_SIZE; ++i) {
        for (int j = -BLOB_SIZE; j <= BLOB_SIZE; ++j) {
            int x = X + i;
            int y = Y + j;

            Particle& p = grid[y*GRID_WIDTH + x];
            if(isWithinBounds(x,y))
            {
                // if(p.r == 0 || p.g !=0 || p.b != 0) {
                //     p.turnSand(); //reset if already has sand
                // }
                if(p.r != 0 || p.g !=0 || p.b != 0) {
                    p = Particle(); //reset if already has sand
                }
                else if (p.r == 0 || p.g ==0 || p.b == 0) {
                    p.turnSand();
                    p.vy = static_cast<float>(rand() % 5 + 5) * -1.0f;  // initial upward velocity, random between -5 and -10
                    p.vx = static_cast<float>(rand() % 5 - 2); // random horizontal velocity between -2 and 2
                }
            }
            
        }
    }
}


int main(void)
{
    FsPassedTime();

    double currentTime = 0.0;

    FsOpenWindow(0,0,800,600,1);
    FsPassedTime();
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    srand(time(NULL));

    Grid grid;

    for(;;)
    {
        FsPollDevice();
        auto key=FsInkey();
        if(FSKEY_ESC==key)
        {
            break;
        }

        int lb,mb,rb,mx,my;
        int evt=FsGetMouseEvent(lb,mb,rb,mx,my);

        if(0 != lb)
        {   
            grid.handleMouseClick(mx,my,currentTime);
        }
        
        currentTime += FsPassedTime() / 1000.0;
        if (grid.isActivityTimeoutExpired(currentTime)) {
            continue;
        }

        grid.update(currentTime);

        int wid,hei;
        FsGetWindowSize(wid,hei);
        glViewport(0,0,wid,hei);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, 800, 600, 0, -1, 1);

        grid.render();

        FsSwapBuffers();
        FsSleep(20);
    }
    return 0;
}
