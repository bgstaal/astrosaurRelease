#pragma once

#include "Resources.h"
#include "ofxVoid/setlist/Sequence.h"
#include "ofxVoid/types/Select.h"
#include "ofxVoid/laser/LaserShape.h"
#include "ofxVoid/midi/MidiManager.h"


#pragma mark - Sequence class

class TriggerSequence : public ofxVoid::setlist::Sequence<Resources>, public ofxVoid::midi::MidiManagerListener
{
public:
	
	struct Trigger
	{
		int row = -1;
		int column = -1;
	};
	
	void start(float time);
	void update(float time, float timeStep);
	void stop();
	
	void onMidiMessage(ofxMidiMessage & msg);
	
	bool isNoteEnabled();
	bool isContolEnabled();
	bool isPitchEnabled();
	bool isBeatEnabled();
	
	int getColumn(const ofxMidiMessage & msg, bool laser = false);
  int getColumn(bool laser = false);
  
  bool isNoteSuccession();
  bool isNoteRun();
	
protected:
	
	virtual void _onNoteTrigger(ofxMidiMessage & msg);
	virtual void _onControlTrigger(ofxMidiMessage & msg);
	virtual void _onPitchTrigger(ofxMidiMessage & msg);
	virtual void _onBeatTrigger();
  virtual void _onRunTrigger(int position);
	
	
private:
	
  struct Run
  {
    int pos;
    int total;
    bool reversed;
    float delta;
    float accumulated;
  };
  
  vector<Run> _runs;
  int _succesion;
};


#pragma mark - Factory class

template <class SequenceType>
class TriggerSequenceFactory : public ofxVoid::setlist::SequenceFactory<SequenceType>
{
public:
	string getType()
	{
		return "Trigger";
	}
	
	ofParameterGroup getParameters()
	{
		ofParameterGroup parameters = ofxVoid::setlist::SequenceFactory<SequenceType>::getParameters();
		
    ofParameterGroup beat;
    beat.setName("Beat");
    
		ofParameter<bool> eb("Enable Beat", false);
		beat.add(eb);
    
    ofxVoid::types::Select<string> bs("All");
    bs.options.push_back("All");
    bs.options.push_back("Succession");
    bs.options.push_back("Suc. Reversed");
    
    ofParameter<ofxVoid::types::Select<string>> bsel("Beat Columns", bs);
    beat.add(bsel);
		
		ofParameter<int> nqb("Nth quad beat", 4, 1, 64);
		beat.add(nqb);
		
		ofParameter<int> of("Offset beat", 0, 0, 64);
		beat.add(of);
    
    ofParameterGroup note;
    note.setName("Note");
		
		ofParameter<bool> notee("Enable Note", false);
		note.add(notee);
		
		ofParameter<int> ncha("Note Channel", -1, -1, 16);
		note.add(ncha);

		ofParameter<bool> nsus("Sustain", false);
		note.add(nsus);
    
    ofxVoid::types::Select<string> ns("All");
    ns.options.push_back("All");
    ns.options.push_back("Note");
    ns.options.push_back("Succession");
    ns.options.push_back("Suc. Reversed");
    ns.options.push_back("Run");
    ns.options.push_back("Run Reversed");
    
    ofParameter<ofxVoid::types::Select<string>> nsel("Note Columns", ns);
		note.add(nsel);
    
    ofParameter<int> rlen("Run Length", 4, 1, 100);
    note.add(rlen);
    
    ofParameter<int> rdur("Run Nth Quad", 4, 1, 16);
    note.add(rdur);
    
    ofParameterGroup midi;
    midi.setName("Midi");
		
		ofParameter<bool> cont("Enable Control", false);
		midi.add(cont);
    
		ofParameter<int> ccha("Control Channel", -1, -1, 16);
		midi.add(ccha);
		
		ofParameter<int> ckey("Control Key", -1, -1, 127);
		midi.add(ckey);
		
		ofParameter<bool> pitc("Enable Pitch", false);
		midi.add(pitc);
		
		ofParameter<int> pcha("Pitch Channel", -1, -1, 16);
		midi.add(pcha);
		
		ofParameter<int> pkey("Pitch Key", -1, -1, 127);
		midi.add(pkey);
    
    parameters.add(beat);
    parameters.add(note);
    parameters.add(midi);
    
		return parameters;
	}
};
