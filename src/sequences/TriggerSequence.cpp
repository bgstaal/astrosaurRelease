#include "TriggerSequence.h"

#define NUM_COLUMNS 12
#define LASER_COLUMNS 1


#pragma mark - Sequence class

void TriggerSequence::start(float time)
{
	Sequence::start(time);
	Sequence::defaultStartAnimation();
	getResources()->midi->addListener(this);
	
  _succesion = -1;
}

void TriggerSequence::stop()
{
	Sequence::stop();
	Sequence::defaultStopAnimation();
	getResources()->midi->removeListener(this);
}

void TriggerSequence::update(float time, float timeStep)
{
	Sequence::update(time, timeStep);
	
	if (isBeatEnabled())
	{
    int nth = MAX(1, getParameter<int>("Beat::Nth quad beat").get());
		int off = getParameter<int>("Beat::Offset beat");
		
		if (getResources()->metronome->getOnQuadBeat(nth, off))
		{
			_onBeatTrigger();
		}
	}
  
  for (int i = 0; i < _runs.size(); i++)
  {
    Run & r = _runs[i];
    
    if (r.accumulated >= r.delta)
    {
      r.accumulated = 0.0f;
      
      if (r.reversed)
      {
        r.pos -= 1;
      }
      else
      {
        r.pos += 1;
      }
      
      _onRunTrigger(r.pos);
    }
    
    r.accumulated += timeStep;
    
    if ((r.reversed && r.pos == 0) || (!r.reversed && r.pos == r.total-1))
    {
      _runs.erase(_runs.begin()+i);
      i--;
    }
  }
}

void TriggerSequence::onMidiMessage(ofxMidiMessage& msg)
{
	if (isNoteEnabled() && (msg.status == MIDI_NOTE_ON || msg.status == MIDI_NOTE_OFF) && !isNoteRun())
	{
    int channel = getParameter<int>("Note::Note Channel");
		
		if (channel == -1 || channel == msg.channel)
		{
			_onNoteTrigger(msg);
		}
	}
  
  if (isNoteEnabled() && isNoteRun() && (msg.status == MIDI_NOTE_ON))
  {
    bool rev = getParameter<string>("Note::Note Columns").get() == "Run Reversed";
    int len = getParameter<int>("Note::Run Length");
    float tim = getResources()->metronome->getQuadBeatDuration() * (float)getParameter<int>("Note::Run Nth Quad").get();
    float delta = tim / (float)len;
    
    Run r;
    r.reversed = rev;
    r.pos = rev ? len : -1;
    r.total = len;
    r.delta = delta;
    r.accumulated = delta;
    
    _runs.push_back(r);
  }
	
	if (isContolEnabled() && msg.status == MIDI_CONTROL_CHANGE)
	{
		int channel = getParameter<int>("Midi::Control Channel");
    int key = getParameter<int>("Midi::Control Key");
		
		if (channel == -1 || channel == msg.channel)
		{
			if (key == -1 || key == msg.pitch)
			{
				_onControlTrigger(msg);
			}
		}
	}
	
	if (isPitchEnabled() && msg.status == MIDI_PITCH_BEND)
	{
    int channel = getParameter<int>("Midi::Pitch Channel");
    int key = getParameter<int>("Audion::Midi::Pitch Key");
		
		if (channel == -1 || channel == msg.channel)
		{
			if (key == -1 || key == msg.pitch)
			{
				_onPitchTrigger(msg);
			}
		}
	}
}

bool TriggerSequence::isNoteEnabled()
{
  return getParameter<bool>("Note::Enable Note");
}

bool TriggerSequence::isContolEnabled()
{
  return getParameter<bool>("Midi::Enable Control");
}

bool TriggerSequence::isPitchEnabled()
{
	return getParameter<bool>("Midi::Enable Pitch");
}

bool TriggerSequence::isBeatEnabled()
{
	return getParameter<bool>("Beat::Enable Beat");
}

int TriggerSequence::getColumn(const ofxMidiMessage & msg, bool laser)
{
  string s = getParameter<string>("Note::Note Columns").get();
  int num = laser ? LASER_COLUMNS : NUM_COLUMNS;
  
  if (s == "Note")
  {
    return laser ? msg.pitch % 1 : msg.pitch % 12;
  }
  
  if (s == "Succession")
  {
    if (msg.status == MIDI_NOTE_ON) _succesion++;
    if (_succesion >= num || _succesion < 0) _succesion = 0;
    return _succesion;
  }
  
  if (s == "Suc. Reversed")
  {
    if (msg.status == MIDI_NOTE_ON) _succesion--;
    if (_succesion < 0 || _succesion >= num) _succesion = num-1;
    return _succesion;
  }
  
  return -1;
}

int TriggerSequence::getColumn(bool laser)
{
  string s = getParameter<string>("Beat::Beat Columns").get();
  int num = laser ? LASER_COLUMNS : NUM_COLUMNS;
  
  if (s == "Succession")
  {
    _succesion++;
    if (_succesion >= num) _succesion = 0;
    return _succesion;
  }
  
  if (s == "Suc. Reversed")
  {
    _succesion--;
    if (_succesion < 0) _succesion = num-1;
    return _succesion;
  }
  
  return -1;
}

bool TriggerSequence::isNoteSuccession()
{
  string s = getParameter<string>("Note::Note Columns").get();
  return (s == "Succession" || s == "Suc. Reversed");
}

bool TriggerSequence::isNoteRun()
{
  string s = getParameter<string>("Note::Note Columns").get();
  return (s == "Run" || s == "Run Reversed");
}

#pragma mark - Protected methods

void TriggerSequence::_onNoteTrigger(ofxMidiMessage & msg)
{
	ofLogNotice("TriggerSequence") << "Note trigger is enabled, but [_onNoteTrigger] not overridden";
}

void TriggerSequence::_onControlTrigger(ofxMidiMessage & msg)
{
	ofLogNotice("TriggerSequence") << "Control trigger is enabled, but [_onControlTrigger] not overridden";
}

void TriggerSequence::_onPitchTrigger(ofxMidiMessage & msg)
{
	ofLogNotice("TriggerSequence") << "Pitch trigger is enabled, but [_onPitchTrigger] not overridden";
}

void TriggerSequence::_onBeatTrigger()
{
	ofLogNotice("TriggerSequence") << "Beat trigger is enabled, but [_onBeatTrigger] not overridden";
}

void TriggerSequence::_onRunTrigger(int position)
{
  ofLogNotice("TriggerSequence") << "Run trigger is enabled, but [_onRunTrigger] not overridden";
}
