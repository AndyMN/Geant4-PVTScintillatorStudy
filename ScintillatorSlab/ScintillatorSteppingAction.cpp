#include "ScintillatorSteppingAction.h"
#include "ScintillatorPrimaryGeneratorAction.h"
#include "globals.hh"

#include <iostream>
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"


ScintillatorSteppingAction::ScintillatorSteppingAction(ScintillatorAnalysisManager* AnalysisManager) : G4UserSteppingAction(), _AnalysisManager(AnalysisManager){

}

ScintillatorSteppingAction::~ScintillatorSteppingAction(){
}

void ScintillatorSteppingAction::ResetEventContainers(){
	TotalEnergyDepositEvent = 0;
	TotalOptPhotonsEvent = 0;
	OptPhotonProcesses.clear();
	ParticleNameTrackID.clear();
	OptPhotonProcesses.clear();
	ParticleNameTrackID.clear();
	OptPhotonsParentID.clear();
	InteractionProcesses.clear();
}

std::map<std::string, int> ScintillatorSteppingAction::GetOptPhotonProcesses(){
	return OptPhotonProcesses;
}

std::map<std::string, int> ScintillatorSteppingAction::GetInteractionProcesses(){
	return InteractionProcesses;
}

double ScintillatorSteppingAction::GetTotalEnergyDepositEvent(){
	return TotalEnergyDepositEvent;
}

int ScintillatorSteppingAction::GetTotalOptPhotonsEvent(){
	return TotalOptPhotonsEvent;
}


double ScintillatorSteppingAction::GetPrimaryEnergy(){
	return _PrimaryEnergy;
}

void ScintillatorSteppingAction::SetPrimaryEnergy(){
	G4RunManager* RunManager = G4RunManager::GetRunManager();

	const ScintillatorPrimaryGeneratorAction* PrimaryGeneratorAction = static_cast<const ScintillatorPrimaryGeneratorAction*> (RunManager->GetUserPrimaryGeneratorAction());

	_PrimaryEnergy = PrimaryGeneratorAction->GetParticleEnergy();
}

void ScintillatorSteppingAction::SetPrimaryName(){
	G4RunManager* RunManager = G4RunManager::GetRunManager();

	const ScintillatorPrimaryGeneratorAction* PrimaryGeneratorAction = static_cast<const ScintillatorPrimaryGeneratorAction*> (RunManager->GetUserPrimaryGeneratorAction());

	_PrimaryName = PrimaryGeneratorAction->GetParticleName();
}

std::string ScintillatorSteppingAction::GetPrimaryName(){
	return _PrimaryName;
}

std::map<int, std::string> ScintillatorSteppingAction::GetParticleNameTrackID(){
	return ParticleNameTrackID;
}

std::map<int, int> ScintillatorSteppingAction::GetOptPhotonsParentID(){
	return OptPhotonsParentID;
}

void ScintillatorSteppingAction::UserSteppingAction(const G4Step* Step){
	//std::cout << "In User Stepping Action: " << std::endl;
	G4Track* myTrack = Step->GetTrack();
	int ParentID = myTrack->GetParentID();
	int ID = myTrack->GetTrackID();

	G4int eID = 0;
    const G4Event* evt = G4RunManager::GetRunManager()->GetCurrentEvent();
    if(evt){
		eID = evt->GetEventID();
    }

	std::string ParticleName = (myTrack->GetDefinition())->GetParticleName();

	// Store the ID with the particle Name of the track. Can be used to check which Particle produces Opt Photons
	int CurrentStepNumber = myTrack->GetCurrentStepNumber();
	if (CurrentStepNumber == 1){
		ParticleNameTrackID.insert(std::pair<int, std::string>(ID, ParticleName));
	}


	G4StepPoint* PreStepPoint = Step->GetPreStepPoint();
	G4StepPoint* PostStepPoint = Step->GetPostStepPoint();

	G4Material* PreMaterial = PreStepPoint->GetMaterial();
	std::string PreMaterialName = "none";
	if (PreMaterial){
		PreMaterialName = PreMaterial->GetName();
	}


	G4Material* PostMaterial = PostStepPoint->GetMaterial();
	std::string PostMaterialName = "none";
	if (PostMaterial){
		PostMaterialName = PostMaterial->GetName();
	}



	// Stopping Power and Landau Distribution Analysis and Opt Photon Amount Analysis
	if (ID == 1){
		// Landau and Stopping Power
		if (PreMaterialName == ScintillatorMaterial && PostMaterialName == ScintillatorMaterial){
			if (PreStepPoint->GetKineticEnergy() == _PrimaryEnergy){
				_AnalysisManager->StoppingPowerFill(PreStepPoint->GetKineticEnergy(), PreStepPoint->GetTotalEnergy() - PostStepPoint->GetTotalEnergy(), Step->GetStepLength());
				_AnalysisManager->LandauDistributionFill(PreStepPoint->GetTotalEnergy() - PostStepPoint->GetTotalEnergy(), Step->GetStepLength());
			}
			TotalEnergyDepositEvent += PreStepPoint->GetTotalEnergy() - PostStepPoint->GetTotalEnergy();

		}
	}

	// Secondary Particles Analysis
	if (ID != 1 && ParticleName != "opticalphoton"){
		int CurrentStepNumber = myTrack->GetCurrentStepNumber(); // Dont count every Particle double
		if (CurrentStepNumber == 1 && PreMaterialName == ScintillatorMaterial){
			_AnalysisManager->SecondaryParticlesFill(ParticleName);
			std::string CreationProcess = (myTrack->GetCreatorProcess())->GetProcessName();
			std::cout << "------------------------------------------------------------------" << std::endl;
			std::cout << "Proc: " << CreationProcess << std::endl;
			std::cout << "Parent Particle: " << ParticleNameTrackID[ParentID] << std::endl;
			std::cout << "Parent ID: " << ParentID << std::endl;
			std::cout << "ME: " << ParticleName << std::endl;
			std::cout << "MY ID: " << ID << std::endl;
			std::cout << "Time: " << PreStepPoint->GetGlobalTime() << std::endl;
			std::cout << "Event ID: " << eID << std::endl;
			std::cout << "------------------------------------------------------------------" << std::endl;

			intprocit = InteractionProcesses.find(CreationProcess);
			if (intprocit == InteractionProcesses.end()){
				InteractionProcesses.insert(std::pair<std::string, int>(CreationProcess, 1));
			}else{
				intprocit->second = intprocit->second + 1;
			}
		}
	}

	if (ID != 1 && ParticleName == "opticalphoton"){
		int CurrentStepNumber = myTrack->GetCurrentStepNumber(); // Dont count every Opt Photon in everystep
		if (CurrentStepNumber == 1 && PreMaterialName == ScintillatorMaterial){
			double OptPhotonGlobalTime = PreStepPoint->GetGlobalTime();
			if (OptPhotonGlobalTime < 100){
				_AnalysisManager->OptPhotonWavelengthsFill(PreStepPoint->GetTotalEnergy());
				_AnalysisManager->OptPhotonTimeFill(OptPhotonGlobalTime);

				// Store the process which created the optical photon
				std::string CreationProcess = (myTrack->GetCreatorProcess())->GetProcessName();
				optit = OptPhotonProcesses.find(CreationProcess);
				if (optit == OptPhotonProcesses.end()){
					OptPhotonProcesses.insert(std::pair<std::string, int>(CreationProcess, 1));
				}else{
					optit->second = optit->second + 1;
				}
				if (CreationProcess == "Cerenkov"){
					TotalOptPhotonsEvent++;
				}else if (CreationProcess == "Scintillation"){
					TotalOptPhotonsEvent += 100;
				}

				// Store the ID of the Parent Track from which the optical photon came
				parentit = OptPhotonsParentID.find(ParentID);
				if (parentit == OptPhotonsParentID.end()){
					OptPhotonsParentID.insert(std::pair<int, int>(ParentID, 1));
				}else{
					parentit->second = parentit->second + 1;
				}

				std::cout << "OPT Proc: " << CreationProcess << std::endl;
				std::cout << "Parent ID: " << ParentID << std::endl;
				std::cout << "MY ID: " << ID << std::endl;
				std::cout << "Energy: " << PreStepPoint->GetTotalEnergy() << std::endl;
				std::cout << "Time: " << OptPhotonGlobalTime << std::endl;
				std::cout << "EventID: " << eID << std::endl;
				if (OptPhotonGlobalTime < 0.01){
					std::cout << "THIS IS A FAAAAST OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOONE 1111111111111" << std::endl;
				}

			}
		}
	}



}
