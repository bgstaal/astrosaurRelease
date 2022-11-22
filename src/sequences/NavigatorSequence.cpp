#include "NavigatorSequence.h"


#pragma mark - Public methods

void NavigatorSequence::start(float time)
{
	SetNavigationSequence::start(time);
	getResources()->midi->addListener(this);
}

void NavigatorSequence::stop()
{
	getResources()->midi->removeListener(this);
	SetNavigationSequence::stop();
	SetNavigationSequence::stopped();
}

void NavigatorSequence::onMidiMessage(ofxMidiMessage& msg)
{
  if (getParameter<bool>("Disable Midi").get()) return;
  
	if ((msg.status == MIDI_NOTE_ON || msg.status == MIDI_NOTE_OFF))
	{
		int channel = parameters.get<int>("Note Channel");
		
		if (channel == -1 || channel == msg.channel)
		{
		_onNote(msg);
		}
	}
}


#pragma mark - Protected methods

void NavigatorSequence::_onNote(ofxMidiMessage& msg)
{
	if (msg.status == MIDI_NOTE_ON)
	  {
		SequenceNavigator::NavigatorEvent e;
		e.key = msg.pitch;
		e.isRelative = false;
		this->onNavigate.notify(this, e);
	  }
}







