#include "Physics.h"

std::pair<int, int> Physics::calculateNextPositions(Particle& currentParticle, int x, int y, double soundY, double K_FORCE, double VX_DAMP_FACTOR, double timeConstant, double gravity) 
{
    double force = K_FORCE * soundY;

    currentParticle.vy -= force;
    currentParticle.vy += gravity * timeConstant;

    currentParticle.vx *= VX_DAMP_FACTOR;

    int X_NEXT = x + static_cast<int>(currentParticle.vx * timeConstant);
    int Y_NEXT = y + static_cast<int>(currentParticle.vy * timeConstant);
    
    return std::make_pair(X_NEXT, Y_NEXT);
}