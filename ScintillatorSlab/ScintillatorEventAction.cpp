#include "ScintillatorEventAction.h"
#include "globals.hh"
#include "G4Event.hh"
#include "G4PrimaryVertex.hh"

#include "ScintillatorTrackingAction.h"
#include "ScintillatorSteppingAction.h"
#include "ScintillatorAnalysisManager.h"
#include "G4RunManager.hh"

#include "EventData.h"

#include <iostream>
#include <string>

ScintillatorEventAction::ScintillatorEventAction(ScintillatorAnalysisManager* AnalysisManager): G4UserEventAction(), _AnalysisManager(AnalysisManager){

}

ScintillatorEventAction::~ScintillatorEventAction(){
}

void ScintillatorEventAction::BeginOfEventAction(const G4Event* Event){

	std::cout << "In ScintillatorEventAction::BeginOfEventAction" << std::endl;
	G4RunManager* RunManager = G4RunManager::GetRunManager();

	const ScintillatorSteppingAction* ConstSteppingAction = static_cast<const ScintillatorSteppingAction*> (RunManager->GetUserSteppingAction());
	ScintillatorSteppingAction* SteppingAction = const_cast<ScintillatorSteppingAction*>(ConstSteppingAction); //Remove constness so we can reset stuff.

	SteppingAction->ResetEventContainers();
}

void ScintillatorEventAction::EndOfEventAction(const G4Event* Event){

	std::cout << "In ScintillatorEventAction::EndOfEventAction" << std::endl;

	G4RunManager* RunManager = G4RunManager::GetRunManager();

	const ScintillatorSteppingAction* ConstSteppingAction = static_cast<const ScintillatorSteppingAction*> (RunManager->GetUserSteppingAction());
	ScintillatorSteppingAction* SteppingAction = const_cast<ScintillatorSteppingAction*>(ConstSteppingAction); //Remove constness so we can reset stuff.

    double TotalEnergyDepositEvent = SteppingAction->GetTotalEnergyDepositEvent();
    int TotalOptPhotonsEvent = SteppingAction->GetTotalOptPhotonsEvent();
	//std::cout << "dE: " << TotalEnergyDepositEvent << " NOpt: " << TotalOptPhotonsEvent << std::endl;
	_AnalysisManager->OptPhotonAmountDepEFill(TotalEnergyDepositEvent, TotalOptPhotonsEvent);

	double PrimaryEnergy = SteppingAction->GetPrimaryEnergy();
	_AnalysisManager->OptPhotonAmountEFill(PrimaryEnergy, TotalOptPhotonsEvent);

	std::map<std::string, int> OptPhotonProcesses = SteppingAction->GetOptPhotonProcesses();
	_AnalysisManager->OptProcessesDepEFill(TotalEnergyDepositEvent, OptPhotonProcesses);
	_AnalysisManager->OptProcessesEFill(PrimaryEnergy, OptPhotonProcesses);
	_AnalysisManager->DepEkinFill(PrimaryEnergy, TotalEnergyDepositEvent);

	std::map<std::string, int> InteractionProcesses = SteppingAction->GetInteractionProcesses();
	_AnalysisManager->IntProcessesEFill(PrimaryEnergy, InteractionProcesses);

	std::map<int, std::string> ParticleNameTrackID = SteppingAction->GetParticleNameTrackID();
	std::map<int, int> OptPhotonsParentID = SteppingAction->GetOptPhotonsParentID();

	_AnalysisManager->OptParentNameDepEFill(TotalEnergyDepositEvent, ParticleNameTrackID, OptPhotonsParentID);
	_AnalysisManager->OptParentNameEFill(PrimaryEnergy, ParticleNameTrackID, OptPhotonsParentID);

}



