#pragma once

#include "ofxVoid/ui/Scene3d.h"
#include "../utils/Resources.h"


class Simulator : public ofxVoid::ui::Scene3d
{
public:
  
  ofParameterGroup parameters;
  
  Simulator(shared_ptr<Resources> resources);
  
  
protected:

	shared_ptr<Resources> _resources;
  
  virtual void _draw();
  
};
