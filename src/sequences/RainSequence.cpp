#include "RainSequence.h"
#include "ofxVoid/laser/LaserShape.h"


#pragma mark - Sequence class

void RainSequence::start(float time)
{
	float x = 0;
	int n = 10;
	_internalTime = 0.0f;

	for (int i = 0; i < 20; i++)
	{
		Mover m;
		m.rat = ofRandom(-1, 0);
		m.lineIndex = i;
		_initiateParams(m);
		_movers.push_back(m);
	}

	Sequence::start(time);
	Sequence::defaultStartAnimation();
}

void RainSequence::_initiateParams(Mover& m)
{
	m.speed = ofRandom(getParameter<float>("Min Velocity"), getParameter<float>("Max Velocity"));
	m.length = ofRandom(getParameter<float>("Min Length"), getParameter<float>("Max Length"));
}

void RainSequence::stop()
{
	Sequence::stop();
	Sequence::defaultStopAnimation();
}

void RainSequence::update(float time, float delta)
{
	Sequence::update(time, delta);

	for (auto& m : _movers)
	{
		m.rat += delta * m.speed * getParameter<float>("Speed");
		if (m.rat > (1.0 + m.length))
		{
			m.rat = 0;
			_initiateParams(m);
		}
	}

	_internalTime += delta * getParameter<float>("Speed");
	_noiseTime += delta * getParameter<float>("Noise Speed");
}

void RainSequence::draw()
{
	auto lasers = getResources()->laserController->getLasers();
	float rot = getParameter<float>("Rotation");
	int nLines = getParameter<int>("Num Lines");
	float totalWidth = getParameter<float>("Total Width");
	float noiseScale = getParameter<float>("Noise Scale");
	float noiseStrength= getParameter<float>("Noise Strength");
	float projOffset = getParameter<float>("Projector Offset Value");

	ofFloatColor pColor = getParameter<ofFloatColor>("Color").get();
	pColor.setBrightness(pColor.getBrightness() * _alpha);

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

			for (int j = 0; j < nLines; j++)
			{
				int n = 100;
				float x = -(totalWidth*.5f) + ((j / (float)(nLines - 1)) * totalWidth);

				for (int k = 0; k < n; k++)
				{
					float y = -0.5 + (k / (float)(n - 1));
					x += ofSignedNoise((j * 0.1) + (i * projOffset), y * noiseScale, _noiseTime) * noiseStrength * 0.1;

					if (k == 0)
					{
						s.path.moveTo(x, y);
					}
					else
					{
						s.path.lineTo(x, y);
					}
				}
			}

			//l->addShapeToCurrentFrame(s);

			ofxVoid::laser::LaserShape s2 = s;
			s2.path.clear();

			
			for (int j = 0; j < nLines; j++)
			{
				auto& m = _movers[j];

				float rat = m.rat;
				auto line = s.path.getOutline()[j];
				float lStep = m.length / 9.0f;

				if (rat >= 0.0)
				{
					for (int i = 0; i < 10; i++)
					{
						auto p = line.getPointAtPercent(rat - (i * lStep));

						if (i == 0)
						{
							s2.path.moveTo(p);
						}
						else
						{
							s2.path.lineTo(p);
						}
					}
				}
			}

			l->addShapeToCurrentFrame(s2);
		}

		i++;
	}
}