#pragma once

#include "ofxVoid/setlist/Sequence.h"
#include "../LaserController.h"


class Resources : public ofxVoid::setlist::SequenceResourcesAbstract
{
public:

	shared_ptr<LaserController> laserController;
  
};

