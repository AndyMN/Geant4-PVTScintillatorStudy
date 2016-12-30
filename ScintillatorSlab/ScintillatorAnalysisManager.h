#ifndef SCINTILLATORANALYSISMANAGER_H
#define SCINTILLATORANALYSISMANAGER_H

#include "../FileParser/FileParser/FileParser.h"
#include "TH1F.h"
#include <map>

class ScintillatorAnalysisManager{


public:
	ScintillatorAnalysisManager(FileParser& MyFileParser);
	~ScintillatorAnalysisManager();

	void EnergyContainerReset();
	void SetScintillatorThickness(int ScintillatorThickness);
	void SetPrimaryName(std::string PrimaryName);
	void SetParticleEnergyStringTitle(std::string ParticleEnergyStringTitle);
	void SetParticleEnergyStringSave(std::string ParticleEnergyStringSave);

	void StoppingPowerFill(double Ekin, double dE, double dr);
	void StoppingPowerDraw();

	void SecondaryParticlesFill(std::string SecondaryParticleName);
	void SecondaryParticlesDraw();

	void LandauDistributionFill(double dE, double dr);
	void LandauDistributionDraw();

	void OptPhotonWavelengthsFill(double Etot);
	void OptPhotonWavelengthsDraw();

	void OptPhotonTimeFill(double OptPhotonGlobalTime);
	void OptPhotonTimeDraw();

	void OptPhotonAmountDepEFill(double EDeposit, int NumOptPhotons);
	void OptPhotonAmountDepEDraw();

	void OptProcessesDepEFill(double EDeposit, std::map<std::string, int> OptPhotonProcesses);
	void OptProcessesDepEDraw();

	void OptProcessesEFill(double Ekin, std::map<std::string, int> OptPhotonProcesses);
	void OptProcessesEDraw();

	void OptPhotonAmountEFill(double Ekin, int NumOptPhotons);
	void OptPhotonAmountEDraw();

	void OptParentNameDepEFill(double EDeposit, std::map<int, std::string> ParticleNameTrackID, std::map<int, int> OptPhotonsParentID);
	void OptParentNameDepEDraw();

	void DepEkinFill(double Ekin, double EDeposit);
	void DepEkinDraw();

	void OptParentNameEFill(double Ekin, std::map<int, std::string> ParticleNameTrackID, std::map<int, int> OptPhotonsParentID);
	void OptParentNameEDraw();

	void IntProcessesEFill(double Ekin, std::map<std::string, int> InteractionProcesses);
	void IntProcessesDraw();

	void DrawHistogram(TH1F* Histogram, std::string CanvasName, std::string SaveName = "test.png", std::string DrawOptions = "", bool LogCheck = false);

private:

	std::string ParticleName;
	std::string _ParticleEnergyStringTitle;
	std::string _ParticleEnergyStringSave;
	int ScintillatorThickness;

	double correct_round(double NumToRound, int SignificantDigits = 1);

	std::string ScintillatorMaterial = "G4_PLASTIC_SC_VINYLTOLUENE";
	int NumEvents;

	// Maps with the Primary Name as KEY and a boolean as value which determines if we do the mentioned analysis on that or not
	std::map<std::string, bool> StoppingPowerAnalysis;
	std::map<std::string, bool> LandauDistributionAnalysis;
	std::map<std::string, bool> SecondaryParticlesAnalysis;
	std::map<std::string, bool> PhotonWavelengthsAnalysis;
	std::map<std::string, bool> OptPhotonTimeAnalysis;
	std::map<std::string, bool> OptPhotonAmountEAnalysis;
	std::map<std::string, bool> OptPhotonAmountdEAnalysis;
	std::map<std::string, bool> OptProcessesdEAnalysis;
	std::map<std::string, bool> OptProcessesEAnalysis;
	std::map<std::string, bool> OptParentNamedEAnalysis;
	std::map<std::string, bool> OptParentNameEAnalysis;



	// Stopping Power Analysis Variables.
	std::map<double, double> StoppingPowerMap;
	std::map<double, int> DivisorMap;


	// Secondary Particles Analysis Variables.
	std::map<std::string, int> SecondaryParticlesMap;


	// Landau Distribution Analysis Variables.
	std::vector<double> dEdr_values;


	// Opt Photon Wavelengths Analysis Variables.
	std::map<double, int> PhotonWavelengthsMap;


	// Opt Photon Time Analysis Variables.
	std::map<double, int> OptPhotonTimeMap;

	// Opt Photon Per Deposited Energy Variables.
	std::map<double, int> OptPhotonAmountdEMap;
	std::map<double, int> OptPhotonAmountdEDivisionMap;

	// Optical Photon Amount Per Energy Analysis Variables.
	std::map<double, int> OptPhotonAmountMap;

	// Optical Photons per Process per Deposited Energy.
	std::map<double, std::map<std::string, int>> OptProcessesdE;

	// Optical Photons per Process per Start Kinetic Energy.
	std::map<double, std::map<std::string, int>> OptProcessesEkin;

	// Optical Photons per Particle Name per Deposited Energy.
	std::map<double, std::map<std::string, int>> OptParentNamedE;

	// Optical Photons Per Particle Name per Start Kinetic Energy.
	std::map<double, std::map<std::string, int>> OptParentNameEkin;

	// Processes from which secondary particles were created for a given Kinetic Energy
	std::map<double, std::map<std::string, int>> InteractionProcessesEkin;

	// Deposited Energy per Start Kinetic Energy.
	std::map<double, double> DepEkin;
	std::map<double, int> DivisorMap_DepEkin;


};
#endif
