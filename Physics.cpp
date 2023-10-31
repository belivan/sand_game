#include "Physics.h"
#include <utility>
#include "Particle.h"

const double Physics::K_FORCE = 30.0f;
const float Physics::VX_DAMP_FACTOR = 0.9f;
const float Physics::gravity = 9.81f;
const float Physics::timeConstant = 0.3f;

std::pair<int, int> Physics::calculateNextPositions(Particle& currentParticle, int x, int y, double soundY) 
{
    double force = static_cast<double>(K_FORCE * soundY);

    currentParticle.vy -= force;
    currentParticle.vy += gravity * timeConstant;

    currentParticle.vx *= VX_DAMP_FACTOR;

    int X_NEXT = x + static_cast<int>(currentParticle.vx * timeConstant);
    int Y_NEXT = y + static_cast<int>(currentParticle.vy * timeConstant);

    return std::make_pair(X_NEXT, Y_NEXT);
}