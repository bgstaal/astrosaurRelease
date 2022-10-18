#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( )
{
  ofGLFWWindowSettings s;
  s.setSize(1400, 900);
  s.stencilBits = 8;
  
  auto win = ofCreateWindow(s);
  auto app = make_shared<ofApp>();
  
  ofRunApp(win, app);
  ofRunMainLoop();
}
