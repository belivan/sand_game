#ifndef MYSOUND_H
#define MYSOUND_H

#include "fssimplewindow.h"
#include "yssimplesound.h"
#include <string>

class MySound {
private:
    YsSoundPlayer player;
    YsSoundPlayer::SoundData wav;
    std::string fileName;

    const double MAX_DISTANCE = 200.0;
    const double MAX_Y = 32767.0;

    void loadWavFile();
    void playWavFile();

public:
    MySound();
    int getWavCurrentX();
    int getWavCurrentY();
    void drawSoundWave();
    void drawSoundWaveBase();
};

#endif // MYSOUND_H
