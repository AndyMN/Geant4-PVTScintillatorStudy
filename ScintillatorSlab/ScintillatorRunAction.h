#ifndef SCINTILLATORRUNACTION_H
#define SCINTILLATORRUNACTION_H

#include "G4UserRunAction.hh"
#include "G4Run.hh"

#include "TTree.h"

#include <string>

class ScintillatorRunAction : public G4UserRunAction
{
public:
	ScintillatorRunAction();
	virtual ~ScintillatorRunAction();

	virtual void BeginOfRunAction(const G4Run*);
	virtual void EndOfRunAction(const G4Run*);

};

#endif
