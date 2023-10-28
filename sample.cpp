const float dampingFactor = 0.99f;  // Adjust this value to achieve the desired amount of damping

void Grid::update() {
    for (int y = GRID_HEIGHT - 1; y >= 0; --y) {  
        for (int x = 0; x < GRID_WIDTH; ++x) {
            Particle& currentParticle = grid[y*GRID_WIDTH + x];

            // Apply damping to horizontal velocity
            currentParticle.vx *= dampingFactor;
            
            // Horizontal movement based on vx
            int x_next = x + static_cast<int>(currentParticle.vx * timeConstant);
            if (isWithinBounds(x_next, y)) {
                if (!moveParticle(x, y, x_next, y)) {
                    // If particle can't move horizontally, try moving it down or diagonally down
                    int moves[4][2] = {{0, 1}, {1, 1}, {-1, 1}, {0, 0}};  // Possible moves: down, down-right, down-left, no move
                    bool moved = false;
                    for (int i = 0; i < 4; ++i) {
                        x_next = x + moves[i][0];
                        int y_next = y + moves[i][1];
                        if (moveParticle(x, y, x_next, y_next)) {
                            moved = true;
                            break;
                        }
                    }

                    // ... (rest of your code for handling vertical movement)
                }
            }

            // ... (rest of your code for handling vertical movement)
        }
    }
}
