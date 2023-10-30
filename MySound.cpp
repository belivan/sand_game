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

void MySound::drawSoundWaveBase() //would be nice if worked
{
    int wid,hei;
    FsGetWindowSize(wid,hei);
    int axis = hei / 2;

    auto currentTime = player.GetCurrentPosition(wav);
    auto currentPos = wav.SecToNumSample(currentTime);
    int x = static_cast<int>((currentPos / static_cast<double>(wav.GetNumSample() / wav.GetNumChannel())) * wid);
    int y = wav.GetSignedValue16(0, currentPos);
    y = axis - static_cast<int>(y * (axis / 2.0) / 32767.0);

    glColor4f(1.0, 0.0, 0.0, 0.2);
    glBegin(GL_LINES);
    glVertex2i(x, 0);
    glVertex2i(x, hei);
    glEnd();
    
    glColor4f(1.0, 0.0, 0.0, 0.2);
    glLineWidth(1.0);
    glBegin(GL_LINE_STRIP);

    for(long long int x = 0; x < wid; ++x)
    {
        long long int ptr = x * wav.GetNumSamplePerChannel() / wid;
        int y = wav.GetSignedValue16(0, ptr);
        y = axis - static_cast<int>(y * (axis / 2.0) / 32767.0);
        glVertex2i(x, y);
    }
    glEnd();
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
    y_offset = static_cast<int>(y_offset * (axis / 1.5) / 32767.0);

    glColor4f(0.5, 0.0, 0.0, 0.1);
    glLineWidth(1.0);
    glBegin(GL_LINE_STRIP);

    for (long long int x = 0; x < wid; ++x)
    {
        long long int ptr = currentPos + static_cast<long long int>((x-wid/2) * wav.GetNumSamplePerChannel() / (wid * 540));

        // Ensure ptr is within valid range
        ptr = std::max(0LL, std::min(ptr, static_cast<long long int>(wav.GetNumSamplePerChannel() - 1)));

        int y = wav.GetSignedValue16(0, ptr);
        y = axis - static_cast<int>(y * (axis / 2) / 32767.0); //- y_offset;
        glVertex2i(x, y);
    }
    glEnd();
}


void MySound::loadWavFile()
{
    FsChangeToProgramDir();
    std::cout << "Enter Wav File Name:";
    std::getline(std::cin,fileName);

    while(YSOK!=wav.LoadWav(fileName.c_str()))
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