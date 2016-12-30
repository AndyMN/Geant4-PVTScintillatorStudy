#ifndef EVENTDATA_H
#define EVENTDATA_H

#include "TrackData.h"
#include "TObject.h"

class EventData: public TObject{

public:
	int EventID;
	int NumberOfPrimaryVertices;
	std::vector<TrackData> Tracks;

	EventData();
	~EventData();
	void PrintData();

	ClassDef(EventData,1)
};

#endif
