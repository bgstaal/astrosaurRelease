#pragma once

#include "ofxVoid/setlist/Sequence.h"
#include "../utils/Resources.h"


#pragma mark - Sequence class

class NetworkSequence : public ofxVoid::setlist::Sequence<Resources>
{
	vector<glm::vec2> _points;
	deque<bool> _onStates;
	float _internalTime;
	float _timer;
	int _randomSeed;

	void _setupPoints();
	void _numLinesChangeHandler(int& numPoints);
	

public:
	void start(float time);
	void update(float time, float delta);
	void draw();
	void stop();
};


#pragma mark - Factory class

class NetworkSequenceFactory : public ofxVoid::setlist::SequenceFactory<NetworkSequence>
{
public:
	string getType()
	{
		return "Network";
	}
	
	ofParameterGroup getParameters()
	{
		ofParameterGroup parameters = ofxVoid::setlist::SequenceFactory<NetworkSequence>::getParameters();

		auto res = static_pointer_cast<Resources>(resources);
		auto lasers = res->laserController->getLasers();
		
		ofParameterGroup g("Draw to laser");

		for (int i = 0; i < lasers.size(); i++)
		{
			ofParameter<bool> l("Laser " + ofToString(i), true);
			g.add(l);
		}

		parameters.add(g);

		ofParameter<bool> fa("Fade In Alpha", false);
		parameters.add(fa);


		ofParameter<int> numLines("Num Lines", 10, 3, 50);
		parameters.add(numLines);

		ofParameter<int> randomSeed("Random Seed", 0, 0, 255);
		parameters.add(randomSeed);

		ofParameter<float> yScale("Y Scale", .2, .0, .5);
		parameters.add(yScale);

		ofParameter<float> speed("Speed", 1.0, 0.0, 5.0);
		parameters.add(speed);

		ofParameter<float> rotation("Rotation", 0.0, -180.0, 180.0);
		parameters.add(rotation);

		ofParameter<bool> planes("Enable Planes", false);
		parameters.add(planes);

		ofParameter<ofFloatColor> planeColor("Plane Color", ofFloatColor(.5, .5, .5));
		parameters.add(planeColor);

		ofParameter<bool> lines("Enable Lines", false);
		parameters.add(lines);

		ofParameter<ofFloatColor> lineColor("Line Color", ofFloatColor(1.0, 1.0, 1.0));
		parameters.add(lineColor);

		ofParameter<bool> flash("Flash Planes", false);
		parameters.add(flash);
		
		ofParameter<bool> flash2("Flash Lines", false);
		parameters.add(flash2);

		ofParameter<float> fFreq("Flash Frequency", 0.1, 0.01, 1.0);
		parameters.add(fFreq);

		ofParameter<float> fProb("Flash Probability", 0.5, 0.0, 1.0);
		parameters.add(fProb);

		ofParameter<bool> fs("Flash In Sequence", false);
		parameters.add(fs);
		/** add parameters here **/
		
		return parameters;
	}
};
