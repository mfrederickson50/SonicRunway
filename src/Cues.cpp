//
//  Cues.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 1/11/16.
//
//

#include "Cues.hpp"
#include "Audio.hpp"

#include <ctime>

SrCue::SrCue(const SrTime &time) :
    _time(time)
{
    
}

SrTime
SrCue::GetTime() const
{
    return _time;
}

float
SrCue::GetAge(const SrTime &now) const
{
    std::chrono::duration<float> timeSince = now - _time;
    return timeSince.count();
}

////////////////////////////

void
SrCues::Setup(SrAudio *audio)
{
    _audio = audio;
    _initialTime = std::chrono::system_clock::now();
}

void
SrCues::Exit()
{
    
}

void
SrCues::Update(const SrTime &now)
{
    SrTime expiration = now - SrSeconds(5);
    
    // Remove any cues that are outdated
    _RemoveOutdatedCues(&_beatQueue, expiration);
    _RemoveOutdatedCues(&_onsetQueue, expiration);
    
    // Process audio data and push any new cues
    if (_audio->BeatReceived()) {
        std::chrono::duration<float> timeSince = now - _initialTime;
        _beatQueue.push_back(SrCue(now));
    }
    
    if (_audio->OnsetReceived()) {
        std::chrono::duration<float> timeSince = now - _initialTime;
        _onsetQueue.push_back(SrCue(now));
    }
    
}

void
SrCues::_RemoveOutdatedCues(SrQueue *queue,
                            const SrTime & expiration)
{
    while(not queue->empty() and queue->front().GetTime() < expiration) {
        queue->pop_front();
        printf("cue expired.  queue size %zu!\n", queue->size());
    }
}

const SrQueue &
SrCues::GetBeatQueue() const
{
    return _beatQueue;
}

const SrQueue &
SrCues::GetOnsetQueue() const
{
    return _onsetQueue;
}