#ifndef TRACKDATA_H
#define TRACKDATA_H

#include "StepData.h"
#include "TObject.h"
#include <vector>
class TrackData: public TObject{

public:
	int TrackID;
	int ParentTrackID;
	std::string ParticleName;
	std::vector<StepData> Steps;

	void PrintData();
	TrackData();
	~TrackData();


   ClassDef(TrackData,1)
};

#endif
