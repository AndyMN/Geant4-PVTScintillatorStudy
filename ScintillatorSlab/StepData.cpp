#include "StepData.h"

#include <iostream>

ClassImp(StepData)

StepData::StepData(): TObject(), StepID(-5), Pre_Material("none"), Post_Material("none"){
}

StepData::~StepData(){
}

/*void StepData::PrintData(){

	std::cout << "Step ID: " << StepID << std::endl;

	std::cout << "Position: (" << x << ", " << y << ", " << z << ")" << std::endl;

	std::cout << "Velocity: " << Velocity << std::endl;

	std::cout << "Momentum: (" << px << ", " << py << ", " << pz << ")" << std::endl;

	std::cout << "Polarisation: (" << polx << ", " << poly << ", " << polz << ")" << std::endl;

	std::cout << "EKinetic: " << Ekin << std::endl;
	std::cout << "ETotal: " << Etot << std::endl;

	std::cout << "Charge: " << Charge << std::endl;
	std::cout << "Mass: " << Mass << std::endl;
	std::cout << "Magnetic Moment: " << MagneticMoment << std::endl;
	std::cout << "Material: " << Material << std::endl;

	std::cout << "Local Time: " << LocalTime << std::endl;
	std::cout << "Proper Time: " << ProperTime << std::endl;
	std::cout << "GlobalTime: " << GlobalTime << std::endl;


}*/
