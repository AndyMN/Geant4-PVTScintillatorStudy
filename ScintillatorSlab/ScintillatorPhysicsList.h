#ifndef SCINTILLATORPHYSICSLIST_H
#define SCINTILLATORPHYSICSLIST_H

#include "G4VModularPhysicsList.hh"

class ScintillatorPhysicsList : public G4VModularPhysicsList
{
public:
	ScintillatorPhysicsList(bool UseOpticalPhysics = 0);
	virtual ~ScintillatorPhysicsList();

	virtual void SetCuts();
};

#endif
