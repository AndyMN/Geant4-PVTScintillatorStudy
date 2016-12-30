#ifndef SCINTILLATOREVENTACTION_H
#define SCINTILLATOREVENTACTION_H

#include "G4UserEventAction.hh"
#include "ScintillatorAnalysisManager.h"

#include "TTree.h"
#include "EventData.h"

class ScintillatorEventAction : public G4UserEventAction
{
public:
	ScintillatorEventAction(ScintillatorAnalysisManager* AnalysisManager);
	virtual ~ScintillatorEventAction();


	virtual void BeginOfEventAction(const G4Event*);
	virtual void EndOfEventAction(const G4Event*);
private:

	ScintillatorAnalysisManager* _AnalysisManager;
};

#endif
