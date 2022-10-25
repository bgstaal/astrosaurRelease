#include "NetworkSequence.h"
#include "ofxVoid/laser/LaserShape.h"


#pragma mark - Sequence class

void NetworkSequence::start(float time)
{
	_internalTime = 0.0f;
	_randomSeed = getParameter<int>("Random Seed");

	//getParameter<int>("Num Lines").addListener(this, &NetworkSequence::_numLinesChangeHandler);

	_setupPoints();

	Sequence::start(time);
	Sequence::defaultStartAnimation();
}

void NetworkSequence::_numLinesChangeHandler(int &numPoints)
{
	_setupPoints();
}

void NetworkSequence::_setupPoints()
{
	float x = 0;
	int n = getParameter<int>("Num Lines");

	ofSeedRandom(_randomSeed);

	_points.clear();

	for (int i = 0; i < n - 1; i++)
	{
		glm::vec2 p(-0.5, 0.5);

		if (i > 0)
		{
			p.x = -0.5 + (i / (float)(n - 1));
			p.x += ofRandom(-.5, .5) * (1.0 / (float)(n - 1));
			p.y = .5;
		}

		_points.push_back(p);
	}

	_points.push_back(glm::vec2(1.0, 0.5));
}

void NetworkSequence::stop()
{
	Sequence::stop();
	Sequence::defaultStopAnimation();
}

void NetworkSequence::update(float time, float delta)
{
	Sequence::update(time, delta);
	_internalTime += delta * getParameter<float>("Speed");

	if (_points.size() != getParameter<int>("Num Lines") ||
		_randomSeed != getParameter<int>("Random Seed"))
	{
		_randomSeed = getParameter<int>("Random Seed");
		_setupPoints();
	}

	for (auto& p : _points)
	{
		p.y = (ofSignedNoise((p.x * 20.0f) + _internalTime) * getParameter<float>("Y Scale"));
	}
	//ofLog() << "eyah";
}

void NetworkSequence::draw()
{
	auto lasers = getResources()->laserController->getLasers();
	float rot = getParameter<float>("Rotation");
	bool flash = getParameter<bool>("Flash");

	ofFloatColor pColor = getParameter<ofFloatColor>("Plane Color").get();
	ofFloatColor lColor = getParameter<ofFloatColor>("Line Color").get();
	bool enablePlanes = getParameter<bool>("Enable Planes");
	bool enableLines = getParameter<bool>("Enable Lines");

	int i = 0;
	for (auto& l : lasers)
	{
		string toggleName = "Laser " + ofToString(i);
		bool doDraw = getParameter<bool>("Draw to laser::" + toggleName);

		if (doDraw)
		{
			
			//s.path.circle(0, 0, sin(_time) * .3);

			if (enablePlanes)
			{
				for (int i = 0; i < _points.size() - 1; i++)
				{
					ofxVoid::laser::LaserShape s;
					s.color = pColor;
					s.position.x = 0.5;
					s.position.y = 0.5;

					s.path.moveTo(_points[i]);
					s.path.lineTo(_points[i + 1]);

					s.rotation = rot;

					//l->addShapeToCurrentFrame(s);

					if (ofRandom(1.0) > .5 || !flash) l->addShapeToCurrentFrame(s);
				}
			}

			if (enableLines)
			{
				for (auto& p : _points)
				{
					ofxVoid::laser::LaserShape s;
					s.color = lColor;
					s.position.x = 0.5;
					s.position.y = 0.5;

					s.addPoint(p);

					s.rotation = rot;

					//l->addShapeToCurrentFrame(s);
					if (ofRandom(1.0) > .5 || !flash) l->addShapeToCurrentFrame(s);
				}
			}
		}

		i++;
	}
}