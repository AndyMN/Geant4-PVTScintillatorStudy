#include "ScintillatorPhysicsList.h"

#include "G4DecayPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4HadronPhysicsQGSP_BERT_HP.hh"
#include "G4HadronPhysicsFTFP_BERT.hh"
#include "G4OpticalProcessIndex.hh"
#include "G4ProcessManager.hh"
#include "G4VProcess.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4EmLivermorePhysics.hh"

#include "G4OpticalPhysics.hh"

#include <iostream>

ScintillatorPhysicsList::ScintillatorPhysicsList(bool UseOpticalPhysics):G4VModularPhysicsList(){

	std::cout << "In ScintillatorPhysicsList::ScintillatorphysicsList()" << std::endl;

	SetVerboseLevel(1);

	// Decays
	RegisterPhysics(new G4DecayPhysics());

	// EM Physics
	RegisterPhysics(new G4EmStandardPhysics());

	//RegisterPhysics(new G4EmLivermorePhysics());


	// Synchrotron Radiation and GammaNuclear and MuonNuclear Physics
	/*
	 #include "G4SynchrotronRadiation.hh"
	 #include "G4ElectroNuclearBuilder.hh"
	 #include "G4MuNuclearInteraction.hh"
	*/
	RegisterPhysics(new G4EmExtraPhysics());

	// Hadron Elastic Scattering
	/*
	 #include "G4HadronElasticProcess.hh"
	 #include "G4HadronElastic.hh"
	 #include "G4CHIPSElastic.hh"
	 #include "G4ElasticHadrNucleusHE.hh"
	 #include "G4AntiNuclElastic.hh"
	 #include "G4BGGNucleonElasticXS.hh"
	 #include "G4BGGPionElasticXS.hh"
	 #include "G4NeutronElasticXS.hh"
	 #include "G4CrossSectionDataSetRegistry.hh"
	 #include "G4ChipsProtonElasticXS.hh"
	 #include "G4ChipsNeutronElasticXS.hh"
	 #include "G4ComponentAntiNuclNuclearXS.hh"
	 #include "G4CrossSectionElastic.hh"
	*/
	RegisterPhysics(new G4HadronElasticPhysics());

	// Stopping Physics
    RegisterPhysics(new G4StoppingPhysics());

	// Ion Physics
	RegisterPhysics(new G4IonPhysics());

	// Hadron Physics
	/*
	 #include "G4HadronCaptureProcess.hh"
     #include "G4NeutronRadCapture.hh"
     #include "G4NeutronCaptureXS.hh"
     #include "G4NeutronHPCaptureData.hh"
     #include "G4LFission.hh"
	*/
	RegisterPhysics(new G4HadronPhysicsQGSP_BERT_HP());

	// OPTICALPROCESSES
	if (UseOpticalPhysics){
		G4OpticalPhysics* OptPhysics = new G4OpticalPhysics();

		//OptPhysics->Configure(kCerenkov, false);
		//OptPhysics->Configure(kScintillation, false);

		/*OptPhysics->SetTrackSecondariesFirst(kCerenkov, false);
		OptPhysics->SetTrackSecondariesFirst(kScintillation, false);
		OptPhysics->SetTrackSecondariesFirst(kAbsorption, false);
		OptPhysics->SetTrackSecondariesFirst(kRayleigh, false);
		OptPhysics->SetTrackSecondariesFirst(kMieHG, false);
		OptPhysics->SetTrackSecondariesFirst(kBoundary, false);
		OptPhysics->SetTrackSecondariesFirst(kWLS, false);
		OptPhysics->SetTrackSecondariesFirst(kNoProcess, false);*/

		RegisterPhysics(OptPhysics);
	}




}

ScintillatorPhysicsList::~ScintillatorPhysicsList(){
}

void ScintillatorPhysicsList::SetCuts(){
	G4VUserPhysicsList::SetCuts();
	//SetCutValue(15*eV, "e-");
}
