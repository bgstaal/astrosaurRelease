#pragma once

#include "ofxVoid/setlist/Sequence.h"
#include "../utils/Resources.h"


#pragma mark - Sequence class

class RainSequence : public ofxVoid::setlist::Sequence<Resources>
{
	struct Mover
	{
		float rat;
		float speed;
		float length;
		int lineIndex;
	};

	vector <Mover> _movers;
	float _internalTime;
	float _noiseTime;

	void _initiateParams(Mover& m);

public:

	void start(float time);
	void update(float time, float delta);
	void draw();
	void stop();
};


#pragma mark - Factory class

class RainSequenceFactory : public ofxVoid::setlist::SequenceFactory<RainSequence>
{
public:
	string getType()
	{
		return "Rain";
	}
	
	ofParameterGroup getParameters()
	{
		ofParameterGroup parameters = ofxVoid::setlist::SequenceFactory<RainSequence>::getParameters();

		auto res = static_pointer_cast<Resources>(resources);
		auto lasers = res->laserController->getLasers();
		
		ofParameterGroup g("Draw to laser");

		for (int i = 0; i < lasers.size(); i++)
		{
			ofParameter<bool> l("Laser " + ofToString(i), true);
			g.add(l);
		}

		parameters.add(g);


		ofParameter<float> speed("Speed", 1.0, 0.0, 5.0);
		parameters.add(speed);

		ofParameter<float> rotation("Rotation", 0.0, -180.0, 180.0);
		parameters.add(rotation);

		ofParameter<ofFloatColor> color("Color", ofFloatColor(.5, .5, .5));
		parameters.add(color);

		ofParameter<int> numLines("Num Lines", 12, 1, 20);
		parameters.add(numLines);

		ofParameter<float> totW("Total Width", .8, 0.0, 1.0);
		parameters.add(totW);

		ofParameter<float> noiseScale("Noise Scale", 3.0, 0.0, 20.0f);
		parameters.add(noiseScale);

		ofParameter<float> noiseLineOffset("Noise Line Offset", 0.1, 0.0, 1.0);
		parameters.add(noiseLineOffset);

		ofParameter<float> noiseStrength("Noise Strength", .1, 0.0, 1.0);
		parameters.add(noiseStrength);

		ofParameter<float> noiseSpeed("Noise Speed", .8, 0.0, 1.0);
		parameters.add(noiseSpeed);

		ofParameter<float> minVel("Min Velocity", 0.1, 0.0, 1.0);
		parameters.add(minVel);

		ofParameter<float> maxVel("Max Velocity", 0.5, 0.0, 1.0);
		parameters.add(maxVel);

		ofParameter<float> minLen("Min Length", 0.03, 0.0, 1.0);
		parameters.add(minLen);

		ofParameter<float> maxLen("Max Length", 0.1, 0.0, 1.0);
		parameters.add(maxLen);

		
		
		/** add parameters here **/
		
		return parameters;
	}
};
