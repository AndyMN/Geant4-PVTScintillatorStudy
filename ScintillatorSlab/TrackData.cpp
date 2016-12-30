#include "TrackData.h"

#include <iostream>

ClassImp(TrackData)

TrackData::TrackData(): TObject(), TrackID(-5), ParentTrackID(-5), ParticleName("none"){
}

TrackData::~TrackData(){
}

void TrackData::PrintData(){

	std::cout << "TrackID: " << TrackID << std::endl;
	std::cout << "Parent TrackID: " << ParentTrackID << std::endl;
	std::cout << "Particle Name: " << ParticleName << std::endl;
	std::cout << "Number of Steps: " << Steps.size() << std::endl;
}
