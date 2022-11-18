#include "LaserShapeSequence.h"

using namespace ofxVoid::laser;

#pragma mark - Sequence class

void LaserShapeSequence::start(float time)
{
	TriggerSequence::start(time);
  
  _shapes.push_back(LaserShape());
  _rotations.push_back(0);
	_values.resize(1);
}

void LaserShapeSequence::stop()
{
  TriggerSequence::stop();
}

void LaserShapeSequence::update(float time, float timeStep)
{
	TriggerSequence::update(time, timeStep);

	int j = 0;
	for (int i = 0; i < _shapes.size(); i++)
	{

		float speed = parameters.get<float>("rotational velocity");

		float vel = speed * timeStep;
		//vel *= 360.0f;
		//if (mirror) vel *= -1;
		_rotations[i] += vel;

		auto &s = _shapes[i];
		float from = ofClamp(MIN(parameters.get<float>("clip from").get(), parameters.get<float>("clip to").get()), 0.0f, 1.0f);
		float to = ofClamp(MAX(parameters.get<float>("clip from").get(), parameters.get<float>("clip to").get()), 0.0f, 1.0f);
		
		s.setClip(from, to);
		float rot = parameters.get<float>("rotation");

		s.rotation = rot + _rotations[i];

		j++;

		if (j == _shapes.size() / 2.0f)
		{
			j = 0;
		}
	}
	
	float decay = parameters.get<float>("decay");
	_values[0] *= decay;

	for (int i = 0; i < _flashPoints.size(); i++)
	{
		auto& p = _flashPoints[i];
		p.alpha *= decay;

		if (p.alpha < .1) 
		{
			_flashPoints.erase(_flashPoints.begin() + i);
			i--;
		}
	}
}

void LaserShapeSequence::draw()
{
	ofFloatColor col = parameters.get<ofFloatColor>("Color");
	string shape = parameters.get<string>("shape");
	
	auto lasers = getResources()->laserController->getLasers();
	auto size = parameters.get<float>("size");
	float minAlpha = parameters.get<float>("min alpha");
	float maxAlpha = parameters.get<float>("max alpha");
	bool doFlashPoints = getParameter<bool>("flash points");
	string decayType = parameters.get<string>("decay type");

	int i = 0;
	for (auto& l : lasers)
	{
		string toggleName = "Laser " + ofToString(i);
		bool doDraw = getParameter<bool>("Draw to laser::" + toggleName);

		if (doDraw)
		{
			if (getParameter<bool>("audio reactive::enabled"))
			{
				float val = getResources()->audioAnalyzer->getAveragePeak();
				size = ofMap(val, 0.0f, 1.0f, getParameter<float>("audio reactive::min size"), getParameter<float>("audio reactive::max size"));

				col.setSaturation(col.getSaturation() - (getParameter<float>("audio reactive::desaturation ratio") * val));
			}

			if (size < LaserShape::minPointSize)
			{
				size = LaserShape::minPointSize;
			}

			float hs = size * 0.5f;
			auto& s = _shapes[0];
			ofFloatColor c = col;
			bool drawIt = true;

			if (isNoteEnabled() || isBeatEnabled())
			{
				if (decayType == "scale")
				{
					hs *= _values[0];
				}
				else if (decayType == "flash")
				{
					c.a = _values[0] > 0.5f ? maxAlpha : minAlpha;
				}
				else
				{
					c.a = ofMap(_values[0], 0.0f, 1.0f, minAlpha, maxAlpha);
				}

				if (c.a <= 0.01f)
				{
					drawIt = false;
				}

				if (doFlashPoints) drawIt = true;
			}

			if (getParameter<bool>("invisible when small") && size <= LaserShape::minPointSize)
			{
				drawIt = false;
			}

			if (drawIt)
			{
				s.path.clear();
				s.color = ofColor(c.r * 255.0f, c.g * 255.0f, c.b * 255.0f, c.a * 255.0f);

				if (shape == "line")
				{
					s.path.moveTo(-hs, 0.0f);
					s.path.lineTo(hs, 0.0f);

				}
				else if (shape == "rect")
				{
					s.path.rectangle(-hs, -hs, hs * 2.0f, hs * 2.0f);
				}
				else if (shape == "circle")
				{
					s.path.setCircleResolution(parameters.get<int>("circle res"));
					s.path.circle(0, 0, hs);
					s.path.close();
				}
				else if (shape == "cross")
				{
					int circleRes = parameters.get<int>("circle res").get();
					for (int i = 0; i < circleRes; i++)
					{
						float a = TWO_PI * (i / (float)circleRes);
						glm::vec2 pointStart(hs * cos(a), hs * sin(a));
						glm::vec2 pointEnd(-hs * cos(a), -hs * sin(a));
						s.path.moveTo(pointStart);
						s.path.lineTo(pointEnd);
					}
				}
				else if (shape == "star")
				{
					int circleRes = parameters.get<int>("circle res").get();
					int nTips = 2 + circleRes;
					int nStarPts = nTips * 2;
					float angleChangePerPt = TWO_PI / (float)nStarPts;
					float innerRadius = hs / 2.0f;
					float outerRadius = hs;
					float angle = 0;

					for (int i = 0; i < nStarPts; i++) {
						if (i == 0)
						{
							s.path.moveTo(glm::vec2(innerRadius * cos(angle), innerRadius * sin(angle)));
						}
						else if (i % 2 == 0) {
							s.path.lineTo(glm::vec2(innerRadius * cos(angle), innerRadius * sin(angle)));
						}
						else {
							s.path.lineTo(glm::vec2(outerRadius * cos(angle), outerRadius * sin(angle)));
						}
						angle += angleChangePerPt;
					}
					s.path.close();
				}

				s.position.x = getParameter<float>("Pos x");
				s.position.y = getParameter<float>("Pos y");


				if (doFlashPoints)
				{
					auto path = s.path.getOutline()[0];

					for (auto &p : _flashPoints)
					{
						auto s2 = s;
						s2.path.clear();

						auto point = path.getPointAtPercent(p.rat);

						s2.addPoint(point);

						if (decayType == "flash")
						{
							s2.color.a = p.alpha > 0.5f ? 255 : 0;
						}
						else
						{
							s2.color.a = p.alpha * 255;
						}

						l->addShapeToCurrentFrame(s2);
					}
				}
				else
				{
					l->addShapeToCurrentFrame(s);
				}

				
			}
		}

		i++;
	}
}

void LaserShapeSequence::draw(ofxVoid::setlist::SequenceSurface surface)
{

}


#pragma mark - Protected methods

void LaserShapeSequence::_addFlashPoint()
{
	FlashPoint p;
	p.rat = ofRandom(1.0);
	p.alpha = 1.0;

	_flashPoints.push_back(p);
}

void LaserShapeSequence::_onNoteTrigger(ofxMidiMessage & msg)
{
	if (getParameter<bool>("flash points"))
	{
		_addFlashPoint();
	}
	else if (msg.status == MIDI_NOTE_ON)
	{
		_values[0] = 1.0f;
	}
}

void LaserShapeSequence::_onBeatTrigger()
{
	if (getParameter<bool>("flash points"))
	{
		_addFlashPoint();
	}
	else
	{
		_values[0] = 1.0f;
	}
}

void LaserShapeSequence::_onRunTrigger(int position)
{
	if (getParameter<bool>("flash points"))
	{
		_addFlashPoint();
	}
	else
	{
		_values[0] = 1.0f;
	}
}
