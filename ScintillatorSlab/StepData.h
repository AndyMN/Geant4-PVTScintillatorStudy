#ifndef STEPDATA_H
#define STEPDATA_H

#include <string>
#include "TObject.h"

class StepData: public TObject{

public:
	int StepID;

	// PreStepPoint variables
	double Pre_x;
	double Pre_y;
	double Pre_z;

	double Pre_px;
	double Pre_py;
	double Pre_pz;

	double Pre_polx;
	double Pre_poly;
	double Pre_polz;

	double Pre_ProperTime;
	double Pre_LocalTime;
	double Pre_GlobalTime;

	double Pre_Ekin;
	double Pre_Etot;

	double Pre_Mass;
	double Pre_Charge;
	double Pre_MagneticMoment;
	double Pre_Velocity;

	std::string Pre_Material;


	// PostStepPoint Variables
	double Post_x;
	double Post_y;
	double Post_z;

	double Post_px;
	double Post_py;
	double Post_pz;

	double Post_polx;
	double Post_poly;
	double Post_polz;

	double Post_ProperTime;
	double Post_LocalTime;
	double Post_GlobalTime;

	double Post_Ekin;
	double Post_Etot;

	double Post_Mass;
	double Post_Charge;
	double Post_MagneticMoment;
	double Post_Velocity;

	std::string Post_Material;

	// Step Variables

	double TotalEnergyDeposit;
	double NonIonizingEnergyDeposit;
	double DeltaE;

	double Deltax;
	double Deltay;
	double Deltaz;
	double StepLength;

	double Deltat;

	double Deltapx;
	double Deltapy;
	double Deltapz;

	bool IsFirstStepInVolume;
	bool IsLastStepInVolume;

	//void PrintData();
	StepData();
	~StepData();


	ClassDef(StepData,1)
};


#endif
