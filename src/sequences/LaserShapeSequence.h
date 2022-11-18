#pragma once

#include "TriggerSequence.h"
#include "ofxVoid/types/Select.h"


#pragma mark - Sequence class

class LaserShapeSequence : public TriggerSequence
{
public:

	struct FlashPoint
	{
		float rat;
		float alpha;
	};
	
	void start(float time);
	void update(float time, float timeStep);
	void draw();
	void draw(ofxVoid::setlist::SequenceSurface surface);
	void stop();
	
protected:

	vector<FlashPoint> _flashPoints;

	void _onNoteTrigger(ofxMidiMessage & msg);
	void _onBeatTrigger();
	void _onRunTrigger(int position);
	void _addFlashPoint();
	
	
private:
	
	vector<float> _values;
	vector<ofxVoid::laser::LaserShape> _shapes;
	vector<float> _rotations;
};


#pragma mark - Factory class

class LaserShapeSequenceFactory : public TriggerSequenceFactory<LaserShapeSequence>
{
public:
	string getType()
	{
		return "LaserShape";
	}
	
	ofParameterGroup getParameters()
	{
		ofParameterGroup parameters = TriggerSequenceFactory<LaserShapeSequence>::getParameters();

		auto time = parameters.getGroup("Transition").get<float>("Time");
		time.setMax(60.0f);
		

		auto res = static_pointer_cast<Resources>(resources);
		auto lasers = res->laserController->getLasers();

		ofParameterGroup g("Draw to laser");

		for (int i = 0; i < lasers.size(); i++)
		{
			ofParameter<bool> l("Laser " + ofToString(i), true);
			g.add(l);
		}

		parameters.add(g);

		ofParameterGroup audiog;
		audiog.setName("audio reactive");

		ofParameter<bool> audior("enabled", false);
		audiog.add(audior);

		ofParameter<float> desat("desaturation ratio", 0.0f, 0.0f, 2.0f);
		audiog.add(desat);

		ofParameter<float> audiomin("min size", 0.0f, 0.0f, 1.0f);
		audiog.add(audiomin);

		ofParameter<float> audiomax("max size", 1.0f, 0.0f, 2.0f);
		audiog.add(audiomax);

		parameters.add(audiog);

		ofxVoid::types::Select<string> s("line");
		s.options = {"line", "circle", "rect", "cross", "star"};

		ofParameter<int>circleRes("circle res", 100, 3, 100);
		parameters.add(circleRes);

		ofParameter<ofxVoid::types::Select<string>> type("shape", s);
		parameters.add(type);

		ofParameter<ofFloatColor> col("Color", ofFloatColor());
		parameters.add(col);

		ofParameter<float> size("size", 0.5f, 0.0f, 2.0f);
		parameters.add(size);

		ofParameter<float> posX("Pos x", 0.5f, 0.0f, 1.0f);
		parameters.add(posX);

		ofParameter<float> posY("Pos y", 0.5f, 0.0f, 1.0f);
		parameters.add(posY);

		ofParameter<bool> invis("invisible when small", false);
		parameters.add(invis);
		
		ofParameter<float> clipFrom("clip from", 0.0f, 0.0f, 1.0f);
		parameters.add(clipFrom);
		
		ofParameter<float> clipTo("clip to", 1.0f, 0.0f, 1.0f);
		parameters.add(clipTo);

		ofParameter<float> r("rotation", 0.0f, -360.0f, 360.0f);
		parameters.add(r);
		
		ofParameter<float> rvel("rotational velocity", 0.0f, -360.0f, 360.0f);
		parameters.add(rvel);
		
		ofParameter<float> dec("decay", 0.90f, 0.70f, 0.99f);
		parameters.add(dec);

		ofParameter<float> minAl("min alpha", 0.0f, 0.0f, 1.0f);
		parameters.add(minAl);

		ofParameter<float> maxAl("max alpha", 1.0f, 0.0f, 1.0f);
		parameters.add(maxAl);
    
		ofxVoid::types::Select<string> fs("fade");
		fs.options = {"fade", "flash", "scale"};
    
		ofParameter<ofxVoid::types::Select<string>> dtype("decay type", fs);
		parameters.add(dtype);

		ofParameter<bool> flashP("flash points", false);
		parameters.add(flashP);
		
		return parameters;
	}
};
