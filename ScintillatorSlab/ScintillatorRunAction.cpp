#include "ScintillatorRunAction.h"


ScintillatorRunAction::ScintillatorRunAction() : G4UserRunAction(){
}


ScintillatorRunAction::~ScintillatorRunAction(){
}

void ScintillatorRunAction::BeginOfRunAction(const G4Run* Run){
	//std::cout << "In RunAction" << std::endl;
	// Called before entering the event loop.
	// Place to initialize histograms.


}

void ScintillatorRunAction::EndOfRunAction(const G4Run* Run){
	// Called at the very end of the run.
	// Usually used for a simple analysis of the processed run.

}
