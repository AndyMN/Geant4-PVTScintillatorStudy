#ifndef SCINTILLATORPRIMARYGENERATORACTION_H
#define SCINTILLATORPRIMARYGENERATORACTION_H

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"

#include <string>

class ScintillatorPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
	ScintillatorPrimaryGeneratorAction();
	virtual ~ScintillatorPrimaryGeneratorAction();

	virtual void GeneratePrimaries(G4Event*);

	void SetParticleName(std::string ParticleName);
	void SetParticleEnergy(G4double ParticleEnergy);

	std::string GetParticleName() const;
	G4double GetParticleEnergy() const;

private:
	G4ParticleGun* ParticleGun;
	std::string _ParticleName;
	G4double _ParticleEnergy;

};

#endif
