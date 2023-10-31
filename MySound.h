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
    const int SCALING_FACTOR = 200;

    void loadWavFile();
    void playWavFile();

public:
    MySound();
    int getWavCurrentX();
    int getWavCurrentY();
    void drawSoundWave();
    void drawSoundWaveBase();
    double getYNormal(int x);
    double getMusicTime();
    double getMusicLength();
};

#endif // MYSOUND_H
