//
//  App.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 1/13/16.
//
//

#include "App.hpp"
#include "ofApp.h"

#include "ShapePattern.hpp"
#include "FftPattern.hpp"

SrApp::SrApp() :
    _model(),
    _audio(_model.GetSampleRate(), _model.GetBufferSize()),
    _fftBuffer(&_model, &_audio),
    _audioUI(&_audio, 10.0, 10.0),
    _artnet(&_model),
    _previs(&_model)
{
    ofSetFrameRate(_model.GetFramesPerSecond());
    
    ofSoundStreamSetup(_model.GetNumChannels(), _model.GetNumChannels(),
                       _model.GetSampleRate(), _model.GetBufferSize(), 4);
    
    _patterns.push_back(new SrShapePattern(&_model, &_audio));
    _patterns.push_back(new SrFftPattern(&_model, &_audio, &_fftBuffer));
}

SrApp::~SrApp()
{
    for(auto iter = _patterns.begin(); iter != _patterns.end(); iter++) {
        delete *iter;
    }
}

void
SrApp::AudioIn(float * input, int bufferSize, int nChannels)
{
    _audio.AudioIn(input, bufferSize, nChannels);
    _fftBuffer.FftIn(_audio.GetBandsEnergies());
}

void
SrApp::AudioOut(float *output, int bufferSize, int nChannels)
{
    _audio.AudioOut(output, bufferSize, nChannels);
}

void
SrApp::Update()
{
    SrTime now = std::chrono::system_clock::now();
    
    _audio.UpdateEvents(now);
    _fftBuffer.Update();
    
    for(auto iter = _patterns.begin(); iter != _patterns.end(); iter++) {
        SrPattern *pattern = *iter;
        pattern->Update(now);
    }
    
    _audioUI.Update();
    _previs.Update();
    
    std::string fpsStr = "frame rate: " + ofToString(ofGetFrameRate(), 2);
    ofSetWindowTitle(fpsStr);
}

void
SrApp::Draw()
{
    SrTime now = std::chrono::system_clock::now();
    
    _model.Clear();
    _model.BeginDrawing();
    
    for(auto iter = _patterns.begin(); iter != _patterns.end(); iter++) {
        SrPattern *pattern = *iter;
        pattern->Draw(now);
    }
    
    _model.EndDrawing();
    
    ofBackground(40,40,40);
    
    _fftBuffer.Draw(10,150,800,75);
    _model.RenderFrameBuffer(10,250, 400, 75);
    
    _audioUI.Draw();
    
    _previs.Draw(10,350,800,600);
    
    _artnet.UpdateLights();
}