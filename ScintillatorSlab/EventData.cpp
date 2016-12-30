#include "EventData.h"

#include <iostream>


ClassImp(EventData)

EventData::EventData(): TObject(), EventID(-5), NumberOfPrimaryVertices(-5){
}

EventData::~EventData(){
}

void EventData::PrintData(){
	std::cout << "EventID: " << EventID << std::endl;
	std::cout << "Number Of Primary Vertices: " << NumberOfPrimaryVertices << std::endl;
	std::cout << "Number Of Tracks: " << Tracks.size() << std::endl;
}

