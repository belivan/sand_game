#ifndef PHYSICS_H
#define PHYSICS_H

#include <utility>
#include "Particle.h"
#include "MySound.h"

class Physics {
    private:
        static const double K_FORCE; //edit if needed
        static const float VX_DAMP_FACTOR; //edit if needed
        static const float gravity;  // gravity constant
        static const float timeConstant; //time constant
    public:
        static std::pair<int, int> calculateNextPositions(Particle& currentParticle, int x, int y, MySound& sound);
        static std::pair<int, int> calculateNextPositions(Particle& currentParticle, int x, int y);
        void updatePhysics(float xVelocityMultiplier, float yVelocityMultiplier);
        static float K_X;
        static float K_Y;
};

#endif