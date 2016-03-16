//
//  Model.hpp
//  SonicRunway
//
//  Created by Rob Jensen on 1/10/16.
//
//

#ifndef _SR_MODEL_HPP
#define _SR_MODEL_HPP

#include <stdio.h>
#include "ofMain.h"

//
// Model class for the app.  Holds global settings and
// the main frame buffer that store the color of all the lights.
//
//
class SrModel {
public:
    SrModel();
    ~SrModel();
    
    void Update();
    
    int GetSampleRate() const;
    int GetBufferSize() const;
    int GetNumChannels() const;
    float GetBuffersPerSecond() const;
    
    // XXX Station 0 is assumed to be at the sound source, though it
    // probably won't exist in the actual installation.  So this
    // number may be higher than the number of physical stations by
    // one or two.
    int GetNumStations() const;
    int GetLightsPerStation() const;
    
    float GetRunwayLength() const; // in feet
    float GetSpeedOfSound() const; // in feet per second
    float GetArchLength() const;  // in feet
    float GetFramesPerSecond() const;
    
    // XXX maybe replace this with DelayAtStation to prevent
    // assumptions about the distance to the first station??
    float ComputeDelayPerStation() const; // in seconds
    float GetMaxBufferDuration() const; // in seconds
    
    void Clear();
    void BeginDrawing();
    void EndDrawing();
    
    const ofFloatPixels & GetFloatPixels() const;
    void RenderFrameBuffer(float x, float y, float width, float height);
    
private:
    int _sampleRate;
    int _bufferSize;
    int _numChannels;
    float _buffersPerSecond;
    
    int _numStations;
    int _lightsPerStation;
    float _runwayLength;
    float _speedOfSound;
    float _archLength;
    float _framesPerSecond;
    
    
    ofFbo _frameBuffer;
    ofFloatPixels _floatPixelsCache;
};

#endif
