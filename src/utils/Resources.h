#pragma once

#include "ofxVoid/setlist/Sequence.h"
#include "../LaserController.h"
#include "ofxVoid/audio/AudioAnalyzer.h"
#include "ofxVoid/midi/MidiManager.h"
#include "ofxVoid/midi/Metronome.h"

class Resources : public ofxVoid::setlist::SequenceResourcesAbstract
{
public:

	shared_ptr<LaserController> laserController;
	shared_ptr<ofxVoid::audio::AudioAnalyzer> audioAnalyzer;
	shared_ptr<ofxVoid::midi::MidiManager> midi;
	shared_ptr<ofxVoid::midi::Metronome> metronome;

};