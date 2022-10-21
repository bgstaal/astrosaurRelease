#pragma once

#include "ofxVoid/setlist/Sequence.h"
#include "../LaserController.h"
#include "ofxVoid/audio/AudioAnalyzer.h"


class Resources : public ofxVoid::setlist::SequenceResourcesAbstract
{
public:

	shared_ptr<LaserController> laserController;
	shared_ptr<ofxVoid::audio::AudioAnalyzer> audioAnalyzer;

};