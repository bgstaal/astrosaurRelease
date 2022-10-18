#pragma once

#include "ofMain.h"
#include "ofxVoid/ui/CellLayout.h"
#include "ofxVoid/setlist/SetList.h"
#include "ofxVoid/setlist/SetListView.h"

#include "ui/Simulator.h"
#include "utils/Resources.h"

#include "sequences/TestSequence.h"
#include "LaserController.h"


class ofApp : public ofBaseApp
{
  
public:
  
  void setup();
  void update();
  void draw();
  void exit();

  void keyPressed(int key);
  void keyReleased(int key);
  void windowResized(int w, int h);
  
  
private:
  
  float _time;
  
  shared_ptr<ofxVoid::ui::CellLayout> _ui;
  shared_ptr<ofxVoid::setlist::SetList> _setlist;
  shared_ptr<LaserController> _laserController;
  shared_ptr<Resources> _resources;
  
  void _loadSettings();
  void _saveSettings();
  
};
