#include "AudioWaveSequence.h"
#include "ofxVoid/laser/LaserShape.h"


#pragma mark - Sequence class

void AudioWaveSequence::start(float time)
{
	float x = 0;
	int n = 100;
	_internalTime = 0.0f;

	for (int i = 0; i < n - 1; i++)
	{
		glm::vec2 p(-0.5, 0.5);

		if (i > 0)
		{
			p.x = -0.5 + (i / (float)(n - 1));
			//.x += ofRandom(-.5, .5) * (1.0 / (float)(n - 1));
			p.y = .5;
		}

		_points.push_back(p);
	}

	_points.push_back(glm::vec2(1.0, 0.5));

	Sequence::start(time);
	Sequence::defaultStartAnimation();
}

void AudioWaveSequence::stop()
{
	Sequence::stop();
	Sequence::defaultStopAnimation();
}

void AudioWaveSequence::update(float time, float delta)
{
	Sequence::update(time, delta);
	_internalTime += delta * getParameter<float>("Speed");

	auto peaks = getResources()->audioAnalyzer->getFftPeakData();
	peaks = getResources()->audioAnalyzer->getFftRawData();
	peaks.resize(peaks.size() * .5);
	vector<float> peaks2;
	int len = peaks.size();

	for (int i = len-1; i >= 0; i--)
	{
		peaks2.push_back(peaks[i]);
	}

	for (int i = len - 1; i >= 0; i--)
	{
		peaks2.push_back(peaks2[i]);
	}

	peaks = peaks2;

	for (auto& val : peaks)
	{
		val = 1.0 - powf(1.0 - val, 20.0);
	}


	//getResources()->audioAnalyzer->getF
	float yScale = getParameter<float>("Y Scale");

	float bell = 10.0f - getParameter<float>("Bell Curve Width");
	bool audio = getParameter<bool>("Use Audio");

	for (auto& p : _points)
	{
		float x = p.x;
		x = ofMap(x, getParameter<float>("X From"), getParameter<float>("X To"), 0.0f, 1.0f);

		if (audio)
		{
			int index = ofMap(x, -.5, .5, 0, peaks.size(), true);
			float val = peaks[index];

			p.y = ofClamp(val * yScale * 0.5, 0.0, 1.0);
		}
		else
		{
			p.y = (ofSignedNoise((x * getParameter<float>("Noise Scale")) + _internalTime, 5.0f) * getParameter<float>("Y Scale"));
			p.y *= (ofSignedNoise((x * getParameter<float>("Noise Scale") * .1) + _internalTime, 5.0f) * getParameter<float>("Y Scale"));
			float sign = 1.0f;
			if (p.y < 0) sign = -1.0f;
			//p.y = powf(p.y, 2.0f) * 2.0f * sign;
		}

		//TODO: choose range on line where the distortion should be placed
		float z = ofMap(x, -.5, .5, -bell, bell);
		p.y *= expf(-powf(z, 2.0f) / 2.0f) / sqrtf(TWO_PI);

		if (getParameter<bool>("Audio Scale"))
		{
			p.y *= getResources()->audioAnalyzer->getAveragePeak();
		}
	}
	//ofLog() << "eyah";
}

void AudioWaveSequence::draw()
{
	auto lasers = getResources()->laserController->getLasers();
	float rot = getParameter<float>("Rotation");

	ofFloatColor pColor = getParameter<ofFloatColor>("Color").get();

	int i = 0;
	for (auto& l : lasers)
	{
		string toggleName = "Laser " + ofToString(i);
		bool doDraw = getParameter<bool>("Draw to laser::" + toggleName);

		if (doDraw)
		{
			ofxVoid::laser::LaserShape s;
			s.color = pColor;
			s.position.x = 0.5;
			s.position.y = 0.5;
			s.rotation = rot;

			s.path.moveTo(_points[0]);

			for (int i = 0; i < _points.size() - 1; i++)
			{
				s.path.lineTo(_points[i]);
			}

			l->addShapeToCurrentFrame(s);

		}

		i++;
	}
}