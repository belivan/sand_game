#include "Grid.h"
#include <random>
#include <limits>
#include <math.h>
#include <iostream>
#include "fssimplewindow.h"
#include "Physics.h"

Grid::Grid() : lastActivityTime(0.0), PARTICLE_COUNT(0){ //change if needed and remove
    GRID_WIDTH = 1024 / PIXELS_PER_GRID_CELL;
    GRID_HEIGHT = 768 / PIXELS_PER_GRID_CELL;

    grid = new Particle[GRID_WIDTH*GRID_HEIGHT];
    for(int i = 0; i < GRID_WIDTH*GRID_HEIGHT; ++i) {
        grid[i] = Particle();
    }
}


Grid::~Grid(){
    delete[] grid;
}

void Grid::reset() {
    delete[] grid;
    grid = new Particle[GRID_WIDTH*GRID_HEIGHT];
    for(int i = 0; i < GRID_WIDTH*GRID_HEIGHT; ++i) {
        grid[i] = Particle();
    }
    PARTICLE_COUNT = 0;
}

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

bool Grid::isWithinBounds(int x, int y) {
    return x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT;
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

void Grid::update(MySound& sound, bool vibration_flag) 
{
    for (int y = GRID_HEIGHT - 1; y >= 0; --y) {  
        for (int x = 0; x < GRID_WIDTH; ++x) {
            Particle& currentParticle = grid[y*GRID_WIDTH + x];

            if(currentParticle.r != 0 || currentParticle.g != 0 || currentParticle.b != 0)
            {
                std::pair<int, int> nextXY;
                if(vibration_flag)
                {
                    nextXY = physics.calculateNextPositions(currentParticle, x, y);
                }
                else
                {
                    nextXY = physics.calculateNextPositions(currentParticle, x, y, sound);
                }
                
                int X_NEXT = nextXY.first;
                int Y_NEXT = nextXY.second;

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
    // Draw the particles
    glBegin(GL_QUADS);

    for (int x = 0; x < GRID_WIDTH; ++x) {
        for (int y = GRID_HEIGHT - 1; y >= 0; --y) {
            Particle &p = grid[y*GRID_WIDTH + x];
            if (p.r != 0 || p.g != 0 || p.b != 0) {
               p.drawParticle(x, y, PIXELS_PER_GRID_CELL, GRID_HEIGHT);
            }
        }
    }
    glEnd();
}

void Grid::background()
{
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
}

void Grid::createSand(int mx, int my, int intensity, bool intensity_flag, bool user) {

    int X = mx / PIXELS_PER_GRID_CELL;
    int Y = my / PIXELS_PER_GRID_CELL;

    if(intensity_flag)
    {
        BLOB_SIZE = intensity; //0-10???
    }

    for (int i = -BLOB_SIZE; i <= BLOB_SIZE; ++i) {
        for (int j = -BLOB_SIZE; j <= BLOB_SIZE; ++j) {
            int x = X + i;
            int y = Y + j;

            if(isWithinBounds(x,y))
            {
                Particle& p = grid[y*GRID_WIDTH + x];
                if(!user && p.r == 0 && p.g == 0 && p.b == 0) //pc adds sand
                {
                    p.turnSand();
                    p.vy = static_cast<float>(rand() % 5 + 5) * -1.0f;
                    p.vx = static_cast<float>(rand() % 5 - 2);
                    ++PARTICLE_COUNT;
                }

                if(user && (p.r != 0 || p.g != 0 || p.b != 0)) //user subtracts sand
                {
                    p = Particle();
                    --PARTICLE_COUNT;
                }
            }
        }
    }
}

int Grid::getParticleCount() {
    // std::cout << "Particle Count: " << PARTICLE_COUNT << std::endl;
    return PARTICLE_COUNT;
}

int Grid::getMaxParticles()
{
    return GRID_WIDTH * GRID_HEIGHT;
}
