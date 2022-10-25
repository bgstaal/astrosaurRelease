#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( )
{
  ofGLFWWindowSettings s;
  s.setSize(1920 * 2, 1025 * 2);
  s.stencilBits = 8;
  
  auto win = ofCreateWindow(s);
  auto app = make_shared<ofApp>();
  
  ofRunApp(win, app);
  ofRunMainLoop();
}
