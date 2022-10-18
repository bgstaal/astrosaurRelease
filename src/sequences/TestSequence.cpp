#include "TestSequence.h"
#include "ofxVoid/laser/LaserShape.h"


#pragma mark - Sequence class

void TestSequence::start(float time)
{
	Sequence::start(time);
	Sequence::defaultStartAnimation();
}

void TestSequence::stop()
{
	Sequence::stop();
	Sequence::defaultStopAnimation();
}

void TestSequence::update(float time, float delta)
{
	Sequence::update(time, delta);
	//ofLog() << "eyah";
}

void TestSequence::draw()
{
	ofxVoid::laser::LaserShape s;
	s.path.circle(0, 0, sin(_time) * .3);

	auto lasers = getResources()->laserController->getLasers();

	int i = 0;
	for (auto& l : lasers)
	{
		string toggleName = "Laser " + ofToString(i);
		bool doDraw = getParameter<bool>("Draw to laser::" + toggleName);

		if (doDraw)
		{
			l->addShapeToCurrentFrame(s);
		}

		i++;
	}
}