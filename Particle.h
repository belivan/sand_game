#ifndef PARTICLE_H
#define PARTICLE_H

class Particle {
public:
    int r, g, b;
    float vy;
    float vx;

    Particle();
    Particle(int red, int green, int blue, bool setFlag);

    void turnSand();
    void drawParticle(int x, int y, int PIXELS_PER_GRID_CELL, int GRID_HEIGHT);
};

#endif // PARTICLE_H
