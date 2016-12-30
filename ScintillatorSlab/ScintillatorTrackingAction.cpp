#include "ScintillatorTrackingAction.h"
#include "globals.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4RunManager.hh"
#include "TrackData.h"
#include <iostream>
#include <string>
#include <type_traits>

ScintillatorTrackingAction::ScintillatorTrackingAction() : G4UserTrackingAction(){
}

ScintillatorTrackingAction::~ScintillatorTrackingAction(){
}

void ScintillatorTrackingAction::PreUserTrackingAction(const G4Track* Track){

	/*std::cout << "In ScintillatorTrackingAction::PreUserTrackingAction" << std::endl;
	std::cout << "ParticleName: " << (Track->GetDefinition())->GetParticleName() << std::endl;*/




}


void ScintillatorTrackingAction::PostUserTrackingAction(const G4Track* Track){

	//std::cout << "In ScintillatorTrackingAction::PostUserTrackingAction" << std::endl;


}


