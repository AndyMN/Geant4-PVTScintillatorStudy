#include "ScintillatorConstruction.h"
#include "ScintillatorPrimaryGeneratorAction.h"
#include "ScintillatorPhysicsList.h"
#include "ScintillatorEventAction.h"
#include "ScintillatorRunAction.h"
#include "ScintillatorSteppingAction.h"
#include "ScintillatorTrackingAction.h"
#include "ScintillatorAnalysisManager.h"
#include "../FileParser/FileParser/FileParser.h"

#include "EventData.h"
#include "StepData.h"
#include "TrackData.h"

#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UImanager.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "QGSP_BERT_HP.hh"
#include "G4PhysicalConstants.hh"
#include "G4PhotoElectricEffect.hh"

#include <string>
#include <algorithm>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "TApplication.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TPaveStats.h"
#include "TGraph.h"


void check_and_make_dir(std::string DirectoryName);
std::string correct_to_string(double number, int decimal_places, char period_replacement = '.');
std::string particle_energy_to_string(double ParticleEnergyInMeV, char period_replacement = '.', bool space_modifier = true);
double correct_round(double NumToRound);

int main(int argc, char* argv[]){


	std::string InputFileName = argv[1];
	FileParser MyFileParser = FileParser(InputFileName);

	std::string DataSetPostFix = MyFileParser.DataSetPostFix();
	std::string DataFileDirectory = MyFileParser.DataFileDirectory();
	std::string DataFileOption = MyFileParser.DataFileOption();
	std::vector<std::vector<double>> ERangeStart = MyFileParser.ERangeStart();
	std::vector<std::vector<double>> ERangeStop = MyFileParser.ERangeStop();
	std::vector<std::vector<double>> ERangeStep = MyFileParser.ERangeStep();
	std::vector<double> SecondaryParticles = MyFileParser.SecondaryParticles();
	std::vector<double> StoppingPower = MyFileParser.StoppingPower();
	std::vector<double> LandauDistribution = MyFileParser.LandauDistribution();
	std::vector<double> PhotonWavelengths = MyFileParser.PhotonWavelengths();
	std::vector<double> OptPhotonAnalysis = MyFileParser.OptPhotonTimeAnalysis();
	std::vector<double> OptPhotonAmount = MyFileParser.OptPhotonAmount();
	std::vector<double> OptPhotonAmountdE = MyFileParser.OptPhotonAmountdE();
	std::vector<double> OptPhotonPoisson = MyFileParser.OptPhotonPoisson();
	std::vector<std::string> ParticleTypes = MyFileParser.ParticleTypes();
	std::vector<std::vector<double>> EDiscreteValues = MyFileParser.EDiscreteValues();
	std::vector<double> SlabThickness = MyFileParser.SlabThickness();
	int NumEvents = MyFileParser.NumEvents();
	int GenerateDataFiles = MyFileParser.GenerateDataFiles();
	int MakePlots = MyFileParser.MakePlots();
	int AnalyzeData = MyFileParser.AnalyzeData();
	int SaveHistograms = MyFileParser.SaveHistograms();
	int UseOpticalPhysics = MyFileParser.UseOpticalPhysics();


	TApplication* rootapp = new TApplication("app",&argc,argv);
	gSystem->Load("libTree");
	G4RunManager* RunManager = new G4RunManager;

	ScintillatorPhysicsList* PhysicsList = new ScintillatorPhysicsList(UseOpticalPhysics);
	RunManager->SetUserInitialization(PhysicsList);



	ScintillatorPrimaryGeneratorAction* PrimaryGeneratorAction = new ScintillatorPrimaryGeneratorAction();
	RunManager->SetUserAction(PrimaryGeneratorAction);

	ScintillatorRunAction* RunAction = new ScintillatorRunAction();
	RunManager->SetUserAction(RunAction);

	ScintillatorTrackingAction* TrackingAction = new ScintillatorTrackingAction();
	RunManager->SetUserAction(TrackingAction);




	for (int particle_index = 0; particle_index < ParticleTypes.size(); particle_index++){
		std::string ParticleName = ParticleTypes[particle_index];

		PrimaryGeneratorAction->SetParticleName(ParticleName);

		for (int thickness_index = 0; thickness_index < SlabThickness.size(); thickness_index++){
			G4int ScintillatorThickness = SlabThickness[thickness_index] * mm;

			ScintillatorAnalysisManager AnalysisManager(MyFileParser);
			AnalysisManager.SetScintillatorThickness(ScintillatorThickness);
			AnalysisManager.SetPrimaryName(ParticleName);

			ScintillatorSteppingAction* SteppingAction = new ScintillatorSteppingAction(&AnalysisManager);
			RunManager->SetUserAction(SteppingAction);

			ScintillatorEventAction* EventAction = new ScintillatorEventAction(&AnalysisManager);
			RunManager->SetUserAction(EventAction);


			std::vector<double> ParticleEnergies = MyFileParser.ParticleEnergies(ERangeStart, ERangeStop, ERangeStep, EDiscreteValues, particle_index);


			for (int energy_index = 0; energy_index < ParticleEnergies.size(); energy_index++){
				G4double ParticleEnergy = ParticleEnergies[energy_index] * MeV;
				std::cout << "Particle Energy (Mev): " << ParticleEnergy << std::endl;

				std::string ParticleEnergyStringTitle = particle_energy_to_string(ParticleEnergy, '.', true);
				std::string ParticleEnergyStringSave = particle_energy_to_string(ParticleEnergy, 'p', false);

				AnalysisManager.SetParticleEnergyStringSave(ParticleEnergyStringSave);
				AnalysisManager.SetParticleEnergyStringTitle(ParticleEnergyStringTitle);

				ScintillatorConstruction* Scintillator = new ScintillatorConstruction(ScintillatorThickness);
				RunManager->SetUserInitialization(Scintillator);

				PrimaryGeneratorAction->SetParticleEnergy(ParticleEnergy);
				SteppingAction->SetPrimaryEnergy();

				RunManager->Initialize(); // Start Kernel, Needed before BeamOn

				// Gamma shenanigans
				//G4ParticleDefinition* gamma_particle = G4Gamma::GammaDefinition();
				//G4ProcessManager* gamma_processmngr = gamma_particle->GetProcessManager();
				//gamma_processmngr->SetProcessActivation(1, false); // Photo Electron
				//gamma_processmngr->SetProcessActivation(2, false); // Compton
				//gamma_processmngr->SetProcessActivation(3, false); // Pair Production
				//gamma_processmngr->DumpInfo();

				RunManager->BeamOn(NumEvents); // Number of Events

				AnalysisManager.SecondaryParticlesDraw();
				AnalysisManager.LandauDistributionDraw();
				AnalysisManager.OptPhotonWavelengthsDraw();
				AnalysisManager.OptPhotonTimeDraw();

				AnalysisManager.EnergyContainerReset();
			} // Energy

			AnalysisManager.StoppingPowerDraw();
			AnalysisManager.OptPhotonAmountDepEDraw();
			AnalysisManager.OptPhotonAmountEDraw();
			AnalysisManager.OptProcessesDepEDraw();
			AnalysisManager.OptProcessesEDraw();
			AnalysisManager.OptParentNameDepEDraw();
			AnalysisManager.OptParentNameEDraw();
			AnalysisManager.DepEkinDraw();
			AnalysisManager.IntProcessesDraw();

		} // Thickness
	} // Particle
	delete RunManager;
	rootapp->Terminate();
	return 0;
} // main

void check_and_make_dir(std::string DirectoryName){
	struct stat st = {0};
	if (stat(DirectoryName.c_str(), &st) == -1){
		mkdir(DirectoryName.c_str(), 0700);
	}
}

std::string correct_to_string(double number, int decimal_places, char period_replacement){

	int corrected_decimal_places = 0;
	if (decimal_places <= 0){
		corrected_decimal_places = -1; // If we don't want decimal places, we shouldn't write the "."
									   // If we don't do this then "5.0" becomes "5." but we want "5"
	}else{
		corrected_decimal_places = decimal_places;
	}

	std::string number_to_string = std::to_string(number);

	int pos_of_period = number_to_string.find_first_of(".");

	number_to_string[pos_of_period] = period_replacement;

	// + 1 needed because we need to give the LENGTH of the substring. pos of period gives 1 for 0.0
	// but the LENGTH is actually 2 "0."
	std::string correct_string = number_to_string.substr(0, pos_of_period + corrected_decimal_places + 1);

	return correct_string;
}

std::string particle_energy_to_string(double ParticleEnergyInMeV, char period_replacement, bool space_modifier){
	std::string ParticleEnergyString;
	std::string SpaceModification = space_modifier ? " " : "";
	if (ParticleEnergyInMeV >= 1000){
		ParticleEnergyString = correct_to_string(ParticleEnergyInMeV / 1000, 1, period_replacement) + SpaceModification +"GeV";
	}else if (ParticleEnergyInMeV < 0.0000001){
		ParticleEnergyString = correct_to_string(ParticleEnergyInMeV / 0.000000001, 1, period_replacement) + SpaceModification + "meV";
	}else if (ParticleEnergyInMeV < 0.0001){
		ParticleEnergyString = correct_to_string(ParticleEnergyInMeV / 0.000001, 1, period_replacement) + SpaceModification + "eV";
	}else if (ParticleEnergyInMeV < 0.1){
		ParticleEnergyString = correct_to_string(ParticleEnergyInMeV / 0.001, 1, period_replacement) + SpaceModification +"keV";
	}else{
		ParticleEnergyString = correct_to_string(ParticleEnergyInMeV, 1, period_replacement) + SpaceModification +"MeV";
	}
	return ParticleEnergyString;
}

