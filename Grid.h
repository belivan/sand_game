#ifndef GRID_H
#define GRID_H

#include "Particle.h"
#include "MySound.h"
#include "Physics.h"

class Grid {
    private:
        int PARTICLE_COUNT = 0;
        int GRID_WIDTH = 0;
        int GRID_HEIGHT = 0;
    
        Particle* grid;
        Physics physics;
        
        const int PIXELS_PER_GRID_CELL = 6;
        int BLOB_SIZE = 10;

        //sound variables
        const double MAX_DISTANCE = 200.0;
        const double MAX_Y = 32767.0;

        //activity variables
        double lastActivityTime;
        const double activityTimeout = 0.9; //IMPORTANT


    public:
        Grid();
        ~Grid();

        void reset();

        void render();
        void createSand(int mx, int my, int intensity, bool intensity_flag, bool user);
        //void removeExcessParticles();
        void update(MySound& sound, bool vibration_flag);

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
