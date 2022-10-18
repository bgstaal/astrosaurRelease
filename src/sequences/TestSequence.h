#pragma once

#include "ofxVoid/setlist/Sequence.h"
#include "../utils/Resources.h"


#pragma mark - Sequence class

class TestSequence : public ofxVoid::setlist::Sequence<Resources>
{
public:
	void start(float time);
	void update(float time, float delta);
	void draw();
	void stop();
};


#pragma mark - Factory class

class TestSequenceFactory : public ofxVoid::setlist::SequenceFactory<TestSequence>
{
public:
	string getType()
	{
		return "Test";
	}
	
	ofParameterGroup getParameters()
	{
		ofParameterGroup parameters = ofxVoid::setlist::SequenceFactory<TestSequence>::getParameters();

		auto res = static_pointer_cast<Resources>(resources);
		auto lasers = res->laserController->getLasers();
		
		ofParameterGroup g("Draw to laser");

		for (int i = 0; i < lasers.size(); i++)
		{
			ofParameter<bool> l("Laser " + ofToString(i), true);
			g.add(l);
		}

		parameters.add(g);
		
		/** add parameters here **/
		
		return parameters;
	}
};
