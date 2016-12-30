#ifndef SCINTILLATORTRACKINGACTION_H
#define SCINTILLATORTRACKINGACTION_H

#include "G4UserTrackingAction.hh"
#include "TTree.h"
#include "TrackData.h"

class ScintillatorTrackingAction : public G4UserTrackingAction
{
public:
	ScintillatorTrackingAction();
	virtual ~ScintillatorTrackingAction();

	virtual void PreUserTrackingAction(const G4Track*);
	virtual void PostUserTrackingAction(const G4Track*);



};

#endif
