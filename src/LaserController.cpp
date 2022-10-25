#include "LaserController.h"
#include "ofxVoid/laser/LaserShape.h"
#include "ofxVoid/ui/ui.h"
#include "ofxVoid/ui/Style.h"
#include "ofCamera.h"
#include "ofxVoid/ui/ui.h"

#ifdef TARGET_WIN32
using namespace ofxVoid::laser::pangolin;
#endif
using namespace std;

std::vector<std::shared_ptr<ofxVoid::laser::Laser>> LaserController::getLasers()
{
	return _lasers;
}

void LaserController::setup(float height)
{
  _font.load("fonts/Theinhardt Light.otf", 100, true, false, true);
  _height = height;
  
#ifdef TARGET_WIN32
  _beyond = make_shared<Beyond>();
  _beyond->setup();
  
  cout << "beyond version " << _beyond->getVersion() << " / dll version: " << _beyond->getDllVersion();
#endif
  
  parameters.setName("Laser");

  ofParameter<float> mp("min point size", 0.004f, 0.0f, 0.1f);
  parameters.add(mp);

  ofParameter<int> rc("repeat count", 0, 0, 100);
  parameters.add(rc);
  
  ofParameter<int> trc("terminate repeat count", 15, 0, 100);
  parameters.add(trc);
  
  ofParameter<int> crc("corner repeat count", 10, 0, 100);
  parameters.add(crc);
  
  ofParameter<int> erc("end repeat count", 10, 0, 100);
  parameters.add(erc);
  
  ofParameter<float> cat("corner angle threshold", 30.0f, 0.0f, 180.0f);
  parameters.add(cat);
  
  ofParameter<float> sd("subdivision distance", 0.1f, 0.01f, 0.2f);
  parameters.add(sd);
  
  ofParameter<bool> sf("show frustum", true);
  parameters.add(sf);
  
  ofParameter<bool> sl("show labels", true);
  parameters.add(sl);
  
  ofParameter<float> br("brightness", 1.0f, 0.0f, 1.0f);
  parameters.add(br);
  
  ofParameter<ofVec2f> fov("fov", ofVec2f(60.0f, 60.0f), ofVec2f(20.0f, 20.0f), ofVec2f(60.0f, 60.0f));
  parameters.add(fov);
  
  parameters.parameterChangedE().add([this](ofAbstractParameter &p)
  {
    if (p.getName() == "show frustum")
    {
		for (auto& l : _lasers)
		{
			l->parameters.get<bool>("show frustum").set(parameters.get<bool>("show frustum"));
		}
   }
  }, 0);

  int num = 2;
  float startX =  5000 * (num-1) * -.5;
  
  for (int i = 0; i < num; i++)
  {
	  auto laser = std::make_shared<ofxVoid::laser::Laser>();
	  laser->setRotation(glm::vec3(-90, 0, 0));
	  laser->setPosition(glm::vec3(startX + i * (2000), _height, 0.0f));
	  laser->setFrustumDepth(_height);

	  _lasers.push_back(laser);

	#ifdef TARGET_WIN32
		  int zoneIndex = i;
		  int projectorIndex = i;

		  auto zone = make_shared<BeyondZone>();
		  zone->setup(_beyond, "zone" + ofToString(zoneIndex), zoneIndex, projectorIndex);
		  _zones[laser] = zone;
	#endif


		  /*
	parameters.add(_laser->parameters.get<float>("brightness"));
	parameters.add(_laser->parameters.get<float>("r correction"));
	parameters.add(_laser->parameters.get<float>("g correction"));
	parameters.add(_laser->parameters.get<float>("b correction"));
	*/
  }
}

void LaserController::update(float time, float timeStep)
{
	ofxVoid::laser::LaserShape::minPointSize = parameters.get<float>("min point size");
	ofxVoid::laser::LaserShape::repeatCount = parameters.get<int>("repeat count");
	ofxVoid::laser::LaserShape::terminateRepeatCount = parameters.get<int>("terminate repeat count");
	ofxVoid::laser::LaserShape::cornerRepeatCount = parameters.get<int>("corner repeat count");
	ofxVoid::laser::LaserShape::endRepeatCount = parameters.get<int>("end repeat count");
	ofxVoid::laser::LaserShape::cornerAngleThreshold = parameters.get<float>("corner angle threshold");
	ofxVoid::laser::LaserShape::subDivisionDistance = parameters.get<float>("subdivision distance");
	
	ofVec2f fov = parameters.get<ofVec2f>("fov");

	for (auto& l : _lasers)
	{
		l->brightnessMultiplier = parameters.get<float>("brightness");
		l->setFieldOfVeiw(fov);
		l->update(time, timeStep);
		l->clearCurrentFrame();
	}
}


void LaserController::draw()
{
	shared_ptr<ofxVoid::ui::Style> s = ofxVoid::ui::getDefaultStyle();

	for (auto& l : _lasers)
	{
		l->draw();
	}
}

void LaserController::draw2dLayer(std::shared_ptr<ofCamera> cam, ofRectangle rect)
{
	float space = 10 * ofxVoid::ui::scale;

	if (parameters.get<bool>("show labels"))
	{
		ofPushStyle();
		shared_ptr<ofxVoid::ui::Style> s = ofxVoid::ui::getDefaultStyle();
		auto f = s->getNumeralsFont();

		string hs = "Proj height: " + ofToString(_height);
		auto r = f->getStringBoundingBox(hs, 0, 0);
		f->drawString(hs, rect.width - r.width - space, space * 1.8);
		
		int i = 0;
		
		//_drawLaserLabel(i, _laser, cam, rect);
		
		ofPopStyle();
	}
}

void LaserController::_drawLaserLabel(int i, std::shared_ptr<ofxVoid::laser::Laser> l, std::shared_ptr<ofCamera> cam, ofRectangle rect)
{
	auto p = cam->worldToScreen(l->getPosition() + glm::vec3(0, 240, 0), rect);
	auto f = ofxVoid::ui::getDefaultStyle()->getNumeralsFont();
	float space = 10 * ofxVoid::ui::scale;

	ofSetColor(255);
	string s = ofToString(i) + ":";
#ifdef TARGET_WIN32
	auto z = _zones[l];
	s += "ZONE" + ofToString(z->getZoneIndex()) + "\n";
	s += "PROJ" + ofToString(z->getProjectorIndex() + 1);
#endif
	f->drawString(s, p.x + (20 * ofxVoid::ui::scale), p.y);
	f->drawString(ofToString(i) + ":" + ofToString(l->numFramePoints()) + "p", (space + (i * 60)) * ofxVoid::ui::scale, space * 1.8);
}

void LaserController::send()
{
#ifdef TARGET_WIN32
	for (auto& l : _lasers)
	{
		_zones[l]->sendFrame(l->getCurrentFrame());
	}
#endif
}

