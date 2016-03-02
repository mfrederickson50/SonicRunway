//
//  ShapePattern.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 3/2/16.
//
//

#include "ShapePattern.hpp"
#include "Audio.hpp"

SrShapePattern::SrShapePattern(SrModel * model, SrAudio * audio) :
    SrPattern(model, audio)
{
    
}

SrShapePattern::~SrShapePattern()
{
    // Destroy all shapes
    for(auto iter = _shapes.begin(); iter != _shapes.end(); iter++) {
        delete *iter;
    }
    
    _shapes.clear();
}

void
SrShapePattern::Update(const SrTime & now)
{
    SrTime expiration = now - SrSeconds(3);
    
    // Remove expired shapes
    for(auto iter = _shapes.begin(); iter != _shapes.end(); ) {
        SrShape *shape = *iter;
        if (shape->GetTime() < expiration) {
            iter = _shapes.erase(iter);
            delete shape;
            printf("num shapes %zu\n", _shapes.size());
        } else {
            ++iter;
        }
    }
    
    // Create new shapes for current events
    const std::vector<SrAudio::Event> & events = GetAudio()->GetCurrentEvents();
    
    for(auto iter = events.begin(); iter != events.end(); iter++) {
        SrAudio::Event event = *iter;
        if (event == SrAudio::LowOnset) {
            SrShape *onsetShape =
                new SrOnsetShape(GetModel(), now, ofColor(0, 50, 150, 255), 0.5);
            
            _shapes.insert(onsetShape);
            printf("num shapes %zu\n", _shapes.size());
        }
        if (event == SrAudio::Beat) {
            // XXX using OnsetShape for now
            SrShape *beatShape =
                new SrOnsetShape(GetModel(), now, ofColor(255, 0, 100, 255), 0.5);
            
            _shapes.insert(beatShape);
            printf("num shapes %zu\n", _shapes.size());
        }
    }
    
}

void
SrShapePattern::Draw(const SrTime & now) const
{
    for(auto iter = _shapes.begin(); iter != _shapes.end(); iter++) {
        SrShape *shape = *iter;
        shape->Draw(now);
    }
}
