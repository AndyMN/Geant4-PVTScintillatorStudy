#ifndef SCINTILLATORCONSTRUCTION_H
#define SCINTILLATORCONSTRUCTION_H

#include "G4VUserDetectorConstruction.hh"
#include "G4Material.hh"
#include "globals.hh"

class ScintillatorConstruction : public G4VUserDetectorConstruction
{
public:
	ScintillatorConstruction(G4double ScintillatorThickness);
	virtual ~ScintillatorConstruction();

	virtual G4VPhysicalVolume* Construct();

	void SetThickness(G4double ScintillatorThickness);
	G4double GetThickness();
private:
	G4double _ScintillatorThickness;
	void _SetOpticalPropertiesPVT(G4Material* PVT);
	void _SetOpticalPropertiesAir(G4Material* Air);
};

#endif
