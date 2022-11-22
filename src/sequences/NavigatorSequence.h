#pragma once

#include "ofMain.h"
#include "Resources.h"
#include "ofxVoid/setlist/Sequence.h"

#pragma mark - Sequence class

class NavigatorSequence : public ofxVoid::setlist::SetNavigationSequence<Resources>, public ofxVoid::midi::MidiManagerListener
{
public:
	void start(float time);
	void stop();
	void onMidiMessage(ofxMidiMessage& msg);
	
private:
  
	void _onNote(ofxMidiMessage& msg);
};

/*

  Yo Bjørn! Har ikke testet den, men har gått gjennom koden.
  Dette er en sekvens vi brukte masse på gullbrandson. Parameteren
  'Slot Jump' sier hvor setlist vil navigere til relativt til den
  slot'en navigator sekvensen er plassert i.

  Har ikke mulighet til å teste midi her, men den skal fungere!
*/



#pragma mark - Factory class

class NavigatorSequenceFactory : public ofxVoid::setlist::SetNavigationSequenceFactory<NavigatorSequence>
{
public:
	string getType()
	{
		return "Navigator";
	}
	
	ofParameterGroup getParameters()
	{
		ofParameterGroup parameters = ofxVoid::setlist::SetNavigationSequenceFactory<NavigatorSequence>::getParameters();
		
		ofParameter<bool> enableMidi("Disable Midi", false);
		parameters.add(enableMidi);
		
		ofParameter<int> noteChannel("Note Channel", -1, -1, 16);
		parameters.add(noteChannel);
		
		return parameters;
	}
};
