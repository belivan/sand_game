#ifndef PHYSICS_H
#define PHYSICS_H

#include <utility>

class Particle {
public:
    double vx;
    double vy;
};

class Physics {
public:
    static std::pair<int, int> calculateNextPositions(Particle& currentParticle, int x, int y, double soundY, double K_FORCE, double VX_DAMP_FACTOR, double timeConstant, double gravity);
};

#endif