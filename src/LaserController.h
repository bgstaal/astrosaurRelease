#pragma once
#include "ofGraphics.h"
#include "ofTypes.h"
#include "ofxVoid/laser/Laser.h"
#include "ofJson.h"
#include "ofTrueTypeFont.h"

#ifdef TARGET_WIN32
#include "ofxVoid/laser/pangolin/Beyond.h"
#include "ofxVoid/laser/pangolin/BeyondZone.h"
#endif


class LaserController
{
	ofJson _laserJson;
	std::vector<std::shared_ptr<ofxVoid::laser::Laser>> _lasers;
	//std::vector<std::vector<std::shared_ptr<ofxVoid::laser::Laser>>> _sortedLasers;
	//std::shared_ptr<ofxVoid::laser::Laser> _backLaser;
	//std::shared_ptr<ofxVoid::laser::Laser> _laser;

	ofTrueTypeFont _font;
	void _drawLaserLabel(int index, std::shared_ptr<ofxVoid::laser::Laser> laser, std::shared_ptr<ofCamera> cam, ofRectangle rect);

	float _height;

#ifdef TARGET_WIN32
	std::shared_ptr<ofxVoid::laser::pangolin::Beyond> _beyond;
	std::map<std::shared_ptr<ofxVoid::laser::Laser>, std::shared_ptr<ofxVoid::laser::pangolin::BeyondZone>> _zones;
#endif

public:

	ofParameterGroup parameters;
  
	//std::vector<std::vector<ofPolyline>> backLaserLedLines;
	//std::vector<std::vector<ofPolyline>> backLaserLedOffsetLines;

	std::vector<std::shared_ptr<ofxVoid::laser::Laser>> getLasers();
	//std::vector<std::vector<std::shared_ptr<ofxVoid::laser::Laser>>> getSortedLasers();
	//std::shared_ptr<ofxVoid::laser::Laser> getBackLaser();
	//std::vector<ofPolyline> getBackLaserPolylines();
  
  //void setBackLaserPolylines(std::vector<ofPolyline> polylines);
  //void setup(float height, float width, float depth, float backLaserHeight, float backLaserDistance);
  
	// std::shared_ptr<ofxVoid::laser::Laser> getLaser();

  void setup(float height);
	void update(float time, float timeStep);
	void draw();
	void draw2dLayer(std::shared_ptr<ofCamera> cam, ofRectangle rect);
	void send();
};

