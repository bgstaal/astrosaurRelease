#include "ofApp.h"
#include "ofxTweenzor.h"
#include "ofxVoid/ui/ui.h"
#include "sequences/NetworkSequence.h"
#include "sequences/AudioWaveSequence.h"
#include "sequences/RainSequence.h"
#include "sequences/LaserShapeSequence.h"

#pragma mark - Public methods

//--------------------------------------------------------------
void ofApp::setup()
{
  ofLogToConsole();
  ofxVoid::ui::init(2.0f);
  Tweenzor::init();
  
  _time = ofGetElapsedTimef();
  
  _resources = make_shared<Resources>();
  
  _setlist = make_shared<ofxVoid::setlist::SetList>();
  _setlist->index = 0;

  _laserController = make_shared<LaserController>();
  _laserController->setup(3000.0f);

  _audioAnalyzer = make_shared<ofxVoid::audio::AudioAnalyzer>("AudioAnalyzer", 240 * ofxVoid::ui::scale);
  _audioAnalyzer->setup();

  _midiManager = make_shared<ofxVoid::midi::MidiManager>();
  _midiManager->setup();

  _metronome = make_shared<ofxVoid::midi::Metronome>();
  _metronome->enableMidiClock(_midiManager);

  _resources->audioAnalyzer = _audioAnalyzer;
  _resources->laserController = _laserController;
  _resources->midi = _midiManager;
  _resources->metronome = _metronome;
  
  vector<shared_ptr<ofxVoid::setlist::SequenceFactoryAbstract>> factories;
  factories.push_back(make_shared<TestSequenceFactory>());
  factories.push_back(make_shared<NetworkSequenceFactory>());
  factories.push_back(make_shared<AudioWaveSequenceFactory>());
  factories.push_back(make_shared<RainSequenceFactory>());
  factories.push_back(make_shared<LaserShapeSequenceFactory>());
  
  // Add factories here
  
  for (auto & f : factories)
  {
    f->resources = _resources;
    ofxVoid::setlist::Set::addGlobalSequenceFactory(f);
  }
  
  _loadSettings();
  _setlist->load();
  
  auto sim = make_shared<Simulator>(_resources);
  
  auto view = make_shared<ofxVoid::setlist::SetListView>();
  view->setSetList(_setlist);
  
  _ui = ofxVoid::ui::CellLayout::create(true);
  auto topCell = ofxVoid::ui::CellLayout::create(false);
  auto rightPanel = ofxVoid::ui::Panel::create();

  rightPanel->addComponent(ofxVoid::ui::FpsLabel::create());
  rightPanel->addComponent(ofxVoid::ui::Spacer::create());
  rightPanel->addComponent(_audioAnalyzer);
  rightPanel->addComponent(ofxVoid::ui::Spacer::create());
  rightPanel->addComponent(_metronome);
  rightPanel->addComponent(ofxVoid::ui::Spacer::create());
  rightPanel->addComponent(_midiManager);
  rightPanel->addComponent(ofxVoid::ui::Spacer::create());

  auto laserGroup = ofxVoid::ui::Group::create("Laser Settings", false);
  laserGroup->addComponents(ofxVoid::ui::createComponentsForParameterGroup(_laserController->parameters));
  rightPanel->addComponent(laserGroup);
  rightPanel->addComponent(ofxVoid::ui::Spacer::create());

  auto saveBtn = ofxVoid::ui::Button::create("Save");
  rightPanel->addComponent(saveBtn);

  saveBtn->onMouseClick([this]() 
  {
      _saveSettings();
  });

  topCell->addComponent(sim);
  topCell->addComponent(rightPanel, ofxVoid::ui::RESIZE_RULE_TYPE_STATIC, 250 * ofxVoid::ui::scale);

  _ui->addComponent(topCell);
  _ui->addComponent(view, ofxVoid::ui::RESIZE_RULE_TYPE_RATIO, 0.3);
  _ui->makeRootObject();
  _ui->disableAutoDraw();
  
  windowResized(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::update()
{
  float t = ofGetElapsedTimef();
  float d = t - _time;

  Tweenzor::update(t * 1000);

  _midiManager->update(t, d);
  _metronome->update();

  _laserController->update(t, d);
  _audioAnalyzer->update();
  _setlist->update(t, d);
  _time = t;
}

//--------------------------------------------------------------
void ofApp::draw()
{
  ofBackground(0);
  
  _setlist->draw();
  _laserController->send();
  
  _ui->draw();
}

//--------------------------------------------------------------
void ofApp::exit()
{
  
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    ofLog() << "key: " << key;
    if (key == 32) //space
    {
        if (_metronome->isRunning())
        {
            _metronome->stop();
        }
        else
        {
            _metronome->start();
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
  _ui->setSize(ofVec2f(w, h));
}


#pragma mark - Private methods

//--------------------------------------------------------------
void ofApp::_loadSettings()
{
  ofFile file("settings.json");
  
  if (file.exists())
  {
    ofJson json;
    file >> json;
    
    ofDeserialize(json, _audioAnalyzer->parameters);
    ofDeserialize(json, _laserController->parameters);
    
    ofLogNotice("ofApp") << "settings loaded";
  }
  else
  {
    ofLogNotice("ofApp") << "no settings file found";
  }
}

//--------------------------------------------------------------
void ofApp::_saveSettings()
{
  ofJson json;

  ofSerialize(json, _audioAnalyzer->parameters);
  ofSerialize(json, _laserController->parameters);
  
  ofSaveJson("settings.json", json);
  ofLogNotice("ofApp") << "settings saved";
}
