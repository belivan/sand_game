#include "Physics.h"
#include <utility>
#include "Particle.h"
#include "MySound.h"

const double Physics::K_FORCE = 30.0f;
const float Physics::VX_DAMP_FACTOR = 0.9f;
const float Physics::gravity = 9.81f;
const float Physics::timeConstant = 0.3f;
float Physics::K_X = 1.0;
float Physics::K_Y = 1.0;

std::pair<int, int> Physics::calculateNextPositions(Particle& currentParticle, int x, int y, MySound& sound) 
{
    double soundY = sound.getYNormal(x);
    double force = static_cast<double>(K_FORCE * soundY);

    currentParticle.vy -= force;
    currentParticle.vy += gravity * timeConstant;

    currentParticle.vx *= VX_DAMP_FACTOR;

    currentParticle.vx *= K_X;
    currentParticle.vy *= K_Y;

    int X_NEXT = x + static_cast<int>(currentParticle.vx * timeConstant);
    int Y_NEXT = y + static_cast<int>(currentParticle.vy * timeConstant);

    // if (x % (GRID_WIDTH*GRID_HEIGHT/1000) == 0) {
                //     std::cout << "Information for x = " << x << ": " << std::endl;
                //     std::cout << "Current Y: " << soundY << std::endl;
                //     std::cout << "Force: " << force << std::endl;
                //     std::cout << "Velocity Y: " << currentParticle.vy << std::endl;
                // }

    return std::make_pair(X_NEXT, Y_NEXT);
}

std::pair<int, int> Physics::calculateNextPositions(Particle& currentParticle, int x, int y) 
{
    currentParticle.vy += gravity * timeConstant;
    currentParticle.vx *= VX_DAMP_FACTOR;

    currentParticle.vx *= K_X;
    currentParticle.vy *= K_Y;

    int X_NEXT = x + static_cast<int>(currentParticle.vx * timeConstant);
    int Y_NEXT = y + static_cast<int>(currentParticle.vy * timeConstant);

    // if (x % (GRID_WIDTH*GRID_HEIGHT/1000) == 0) {
                //     std::cout << "Information for x = " << x << ": " << std::endl;
                //     std::cout << "Current Y: " << soundY << std::endl;
                //     std::cout << "Force: " << force << std::endl;
                //     std::cout << "Velocity Y: " << currentParticle.vy << std::endl;
                // }

    return std::make_pair(X_NEXT, Y_NEXT);
}

void Physics::updatePhysics(float xVelocityMultiplier, float yVelocityMultiplier)
{
    //update velocity
    K_X = xVelocityMultiplier;
    K_Y = yVelocityMultiplier;
}