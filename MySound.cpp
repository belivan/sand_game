#include "MySound.h"

#include <iostream> 
#include <GL/gl.h>
#include "fssimplewindow.h"
#include "yssimplesound.h"

MySound::MySound()
{
    loadWavFile();
    playWavFile();
}

void MySound::drawSoundWaveBase()
{
    int wid, hei;
    FsGetWindowSize(wid, hei);
    int axis = hei / 2;

    auto currentTime = player.GetCurrentPosition(wav);
    auto currentPos = wav.SecToNumSample(currentTime);
    int x = static_cast<int>((currentPos / static_cast<double>(wav.GetNumSample() / wav.GetNumChannel())) * wid);
    int y = wav.GetSignedValue16(0, currentPos);
    y = axis - static_cast<int>(y * (axis / 2.0) / 32767.0);

    int min_y = axis, max_y = axis;
    for (long long int x = 0; x < wid; ++x)
    {
        long long int ptr = x * wav.GetNumSamplePerChannel() / wid;
        int y = wav.GetSignedValue16(0, ptr);
        y = axis - static_cast<int>(y * (axis / 5.0) / 32767.0);
        min_y = std::min(min_y, y);
        max_y = std::max(max_y, y);
    }

    glColor4f(0.8, 0.0, 0.0, 0.2);
    glLineWidth(1.0);
    glBegin(GL_LINE_STRIP);

    for (long long int x = 0; x < wid; ++x)
    {
        long long int ptr = x * wav.GetNumSamplePerChannel() / wid;
        int y = wav.GetSignedValue16(0, ptr);
        y = axis - static_cast<int>(y * (axis / 5.0) / 32767.0);
        glVertex2i(x, y);
    }
    glEnd();

    glColor4f(0.0, 0.75, 0.0, 0.2);
    glLineWidth(3.0);
    glBegin(GL_LINES);
    glVertex2i(x, min_y);
    glVertex2i(x, max_y);
    glEnd();
}

double MySound::getMusicTime()
{
    return player.GetCurrentPosition(wav);
}

double MySound::getMusicLength() //edit this one
{
    return 0;
}

void MySound::drawSoundWave()
{
    int wid, hei;
    FsGetWindowSize(wid, hei);
    int axis = hei / 2;

    auto currentTime = player.GetCurrentPosition(wav);

    auto currentPos = wav.SecToNumSample(currentTime);

    int x_current = static_cast<int>((currentPos / static_cast<double>(wav.GetNumSample() / wav.GetNumChannel())) * wid);

    int y_offset = wav.GetSignedValue16(0, currentPos);
    y_offset = static_cast<int>(y_offset * (axis / 2) / 32767.0);

    double scalingFactor = static_cast<double>(wid) / (wav.GetNumSamplePerChannel() * wav.GetNumChannel() * SCALING_FACTOR);

    glColor4f(0.5, 0.0, 0.0, 0.1);
    glLineWidth(1.0);
    glBegin(GL_LINE_STRIP);

    for (long long int x = 0; x < wid; ++x)
    {
        long long int ptr = currentPos - static_cast<long long int>((x-wid/2) * wav.GetNumSamplePerChannel() * scalingFactor);

        // Ensure ptr is within valid range
        ptr = std::max(0LL, std::min(ptr, static_cast<long long int>(wav.GetNumSamplePerChannel() - 1)));

        int y = wav.GetSignedValue16(0, ptr);
        y = axis - static_cast<int>(y * (axis / 2) / 32767.0); //- y_offset;
        glVertex2i(x, y);
    }
    glEnd();
}

double MySound::getYNormal(int x)
{
    x = static_cast<long long int>(x);

    int wid, hei;
    FsGetWindowSize(wid, hei);
    int axis = hei / 2;

    auto currentTime = player.GetCurrentPosition(wav);
    auto currentPos = wav.SecToNumSample(currentTime);
    double scalingFactor = static_cast<double>(wid) / (wav.GetNumSamplePerChannel() * wav.GetNumChannel() * SCALING_FACTOR);
    long long int ptr = currentPos - static_cast<long long int>((x-wid/2) * wav.GetNumSamplePerChannel() * scalingFactor);

    int y = wav.GetSignedValue16(0, ptr);

    return static_cast<double>(y / 32767.0);
}


void MySound::loadWavFile()
{
    FsChangeToProgramDir();
    // std::cout << "Enter Wav File Name:";
    // std::getline(std::cin,fileName);

    while(YSOK!=wav.LoadWav("disfigure.wav"))
    //while(YSOK!=wav.LoadWav(fileName.c_str()))
    {
        std::cout << "Failed to read %s\n";
        std::cout << "Enter Wav File Name:";
        std::getline(std::cin,fileName);
    }
}

void MySound::playWavFile()
{
    player.Start();
    player.PlayBackground(wav);

    int ptr0=0;
    int range=wav.GetNumSamplePerChannel();

    // for(;;)
    // {
    //     FsPollDevice();
    //     player.KeepPlaying();

    //     auto key=FsInkey();
    //     if(FSKEY_ESC==key)
    //     {
    //         break;
    //     }
    // }
}

int MySound::getWavCurrentX()
{
    int wid,hei;
    FsGetWindowSize(wid,hei);

    auto currentTime=player.GetCurrentPosition(wav);
    auto currentPos=wav.SecToNumSample(currentTime);

    int x = static_cast<int>((currentPos/static_cast<double>(wav.GetNumSample()/wav.GetNumChannel()))*wid);

    return x;
}

int MySound::getWavCurrentY()
{
    int wid, hei;
    FsGetWindowSize(wid, hei);
    int axis = hei / 2;

    auto currentTime = player.GetCurrentPosition(wav);
    auto currentPos = wav.SecToNumSample(currentTime);

    int y = wav.GetSignedValue16(0, currentPos);
    y = axis - static_cast<int>(y * (axis / 2.0) / MAX_DISTANCE);

    return y;
}