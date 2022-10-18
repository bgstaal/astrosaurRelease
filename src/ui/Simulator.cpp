#include "Simulator.h"


#pragma mark - Private methods

Simulator::Simulator(shared_ptr<Resources> resources)
: _resources(resources)
{
  parameters.setName("Simulator");
  
  setBackgroundColor(ofColor(15));
  
  getCamera()->setAutoDistance(false);
  getCamera()->setDistance(20000);
  //getCamera()->setPosition(0, 1600, 10000);
  //getCamera()->lookAt(glm::vec3(0, 1000, 0));
  getCamera()->setNearClip(20.0f);
  getCamera()->setFarClip(100000.0f);
  getCamera()->setFov(27.0f);
}


#pragma mark - Protected methods

void Simulator::_draw()
{
  ofPushStyle();
  ofNoFill();
  ofSetPlaneResolution(100, 100);
  ofEnableDepthTest();
  
  ofPushMatrix();
  {
    ofRotateXDeg(90.0f);
    ofSetColor(30);
    ofDrawPlane(0, 0, 200000.0f, 200000.0f);
  }
  ofPopMatrix();
  
  _resources->laserController->draw();
  // DRAW SHIT HERE
  
  ofDisableDepthTest();
  ofPopStyle();
}
