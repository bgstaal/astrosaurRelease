#pragma once

#include "ofxVoid/setlist/Sequence.h"
#include "../utils/Resources.h"


#pragma mark - Sequence class

class AudioWaveSequence : public ofxVoid::setlist::Sequence<Resources>
{
	vector<glm::vec2> _points;
	float _internalTime;

public:
	void start(float time);
	void update(float time, float delta);
	void draw();
	void stop();
};


#pragma mark - Factory class

class AudioWaveSequenceFactory : public ofxVoid::setlist::SequenceFactory<AudioWaveSequence>
{
public:
	string getType()
	{
		return "AudioWave";
	}
	
	ofParameterGroup getParameters()
	{
		ofParameterGroup parameters = ofxVoid::setlist::SequenceFactory<AudioWaveSequence>::getParameters();

		auto res = static_pointer_cast<Resources>(resources);
		auto lasers = res->laserController->getLasers();
		
		ofParameterGroup g("Draw to laser");

		for (int i = 0; i < lasers.size(); i++)
		{
			ofParameter<bool> l("Laser " + ofToString(i), true);
			g.add(l);
		}

		parameters.add(g);

		ofParameter<bool> audio("Use Audio", true);
		parameters.add(audio);

		ofParameter<bool> as("Audio Scale", false);
		parameters.add(as);

		ofParameter<float> yScale("Y Scale", .2, .0, 10.0);
		parameters.add(yScale);

		ofParameter<float> xFrom("X From", 0.0f, 0.0f, 1.0f);
		parameters.add(xFrom);

		ofParameter<float> xTo("X To", 1.0f, 0.0f, 1.0f);
		parameters.add(xTo);

		ofParameter<float> nScale("Noise Scale", 1.0f, 1.0, 100.0f);
		parameters.add(nScale);

		ofParameter<float> speed("Speed", 1.0, 0.0, 5.0);
		parameters.add(speed);

		ofParameter<float> rotation("Rotation", 0.0, -180.0, 180.0);
		parameters.add(rotation);

		ofParameter<float> bell("Bell Curve Width", 5.0, 1.0f, 10.0f);
		parameters.add(bell);
		
		ofParameter<ofFloatColor> planeColor("Color", ofFloatColor(.5, .5, .5));
		parameters.add(planeColor);
		
		return parameters;
	}
};
