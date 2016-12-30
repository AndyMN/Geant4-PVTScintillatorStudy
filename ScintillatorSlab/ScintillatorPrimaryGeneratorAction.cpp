#include "ScintillatorPrimaryGeneratorAction.h"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Box.hh"
#include <algorithm>

ScintillatorPrimaryGeneratorAction::ScintillatorPrimaryGeneratorAction(): G4VUserPrimaryGeneratorAction(), ParticleGun(0){
	G4int NumParticles = 1; // Particles per event
	ParticleGun = new G4ParticleGun(NumParticles);
}



void ScintillatorPrimaryGeneratorAction::SetParticleName(std::string ParticleName){
	_ParticleName = ParticleName;
	std::transform(_ParticleName.begin(), _ParticleName.end(), _ParticleName.begin(), ::tolower);

	G4ParticleTable* ParticleTable = G4ParticleTable::GetParticleTable();
	G4String G4ParticleName(_ParticleName);
	G4ParticleDefinition* Particle = ParticleTable->FindParticle(G4ParticleName);

	ParticleGun->SetParticleDefinition(Particle);
	ParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));


}

void ScintillatorPrimaryGeneratorAction::SetParticleEnergy(G4double ParticleEnergy){
	if (ParticleEnergy <= 0.0){
		std::cerr << "Particle energy cannot be <= 0.0 !" << std::endl;
		std::cerr << "Particle energy set to 6.0 MeV." << std::endl;
		_ParticleEnergy = 6.0 * MeV;
	}else{
		_ParticleEnergy = ParticleEnergy;
	}

	ParticleGun->SetParticleEnergy(_ParticleEnergy);

}

std::string ScintillatorPrimaryGeneratorAction::GetParticleName() const{
	return _ParticleName;
}

G4double ScintillatorPrimaryGeneratorAction::GetParticleEnergy() const{
	return _ParticleEnergy;
}

ScintillatorPrimaryGeneratorAction::~ScintillatorPrimaryGeneratorAction(){
	delete ParticleGun;
}

void ScintillatorPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){

	G4double DetectorZHalfLength = 0;
	// Makes a G4LVStore for this event by calling GetInstance(). Retrieve LV with name "LVName".
	G4LogicalVolume* DetectorLV = G4LogicalVolumeStore::GetInstance()->GetVolume("ScintillatorLogical");
	G4Box* DetectorBox = NULL;
	if(DetectorLV){
		DetectorBox = dynamic_cast<G4Box*>(DetectorLV->GetSolid()); // Get the Solid from the LV and then cast it to a G4Box which is a derived class from the Solid
	}
	if (DetectorBox){
		DetectorZHalfLength = DetectorBox->GetZHalfLength();
	}else{
		G4cerr << "World volume of box not found" << G4endl;
	}

	ParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -DetectorZHalfLength));

	ParticleGun->GeneratePrimaryVertex(anEvent);

}
