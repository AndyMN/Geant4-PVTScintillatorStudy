#ifndef SCINTILLATORSTEPPINGACTION_H
#define SCINTILLATORSTEPPINGACTION_H

#include "G4UserSteppingAction.hh"
#include "TTree.h"
#include "StepData.h"
#include "ScintillatorAnalysisManager.h"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

class ScintillatorSteppingAction : public G4UserSteppingAction
{
public:
	ScintillatorSteppingAction(ScintillatorAnalysisManager* AnalysisManager);
	virtual ~ScintillatorSteppingAction();

	virtual void UserSteppingAction(const G4Step*);

	void SetPrimaryEnergy();
	double GetPrimaryEnergy();

	void SetPrimaryName();
	std::string GetPrimaryName();

	void ResetEventContainers();

	double GetTotalEnergyDepositEvent();
	int GetTotalOptPhotonsEvent();

	std::map<std::string, int> GetOptPhotonProcesses();

	std::map<std::string, int> GetInteractionProcesses();

	std::map<int, std::string> GetParticleNameTrackID();
	std::map<int, int> GetOptPhotonsParentID();

private:
	ScintillatorAnalysisManager* _AnalysisManager;

	double TotalEnergyDepositEvent = 0;
	int TotalOptPhotonsEvent = 0;



	std::map<std::string, int> OptPhotonProcesses;
	std::map<std::string, int>::iterator optit;

	std::map<std::string, int> InteractionProcesses;
	std::map<std::string, int>::iterator intprocit;

	std::map<int, std::string> ParticleNameTrackID;
	std::map<int, int> OptPhotonsParentID;
	std::map<int, int>::iterator parentit;

	G4double _PrimaryEnergy;
	std::string _PrimaryName;
	std::string ScintillatorMaterial = "G4_PLASTIC_SC_VINYLTOLUENE";
};

#endif
