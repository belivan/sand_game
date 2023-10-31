#ifndef GRID_H
#define GRID_H

#include "Particle.h"
#include "MySound.h"
#include "Physics.h"

class Grid {
    private:
        int PARTICLE_COUNT;
        int MAX_PARTICLES;
        const int MAX_PARTICLES_FACTOR = 15; // 1/n of grid size
        int GRID_WIDTH;
        int GRID_HEIGHT;
    
        Particle* grid;
        Physics physics;
        
        const int PIXELS_PER_GRID_CELL = 4;
        const int BLOB_SIZE = 10;

        //sound variables
        const double MAX_DISTANCE = 200.0;
        const double MAX_Y = 32767.0;

        //activity variables
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
        void background();

        //handling activity
        bool isActivityTimeoutExpired(double currentTime) {
        return currentTime - lastActivityTime > activityTimeout;
        }

        int getParticleCount();
        int getMaxParticles();
};

#endif // GRID_H
