#include "Particle.h"
#include <GL/gl.h>

Particle::Particle() : r(0), g(0), b(0), vy(0.0f), vx(0.0f) {}

Particle::Particle(int red, int green, int blue, bool setFlag)
    : r(red), g(green), b(blue), vy(0.0f), vx(0.0f) {}

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