#include "ScintillatorConstruction.h"

#include "G4NistManager.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"


ScintillatorConstruction::ScintillatorConstruction(G4double ScintillatorThickness):G4VUserDetectorConstruction(){
	SetThickness(ScintillatorThickness / 2);
}

ScintillatorConstruction::~ScintillatorConstruction(){
}

void ScintillatorConstruction::SetThickness(G4double ScintillatorThickness){
	if (ScintillatorThickness <= 0.0){
		std::cerr << "Scintillator thickness cannot be <= 0 !" << std::endl;
		std::cerr << "Thickness set to 5.0 cm" << std::endl;
		_ScintillatorThickness = 5.0 * cm;
	}else{
		_ScintillatorThickness = ScintillatorThickness;
	}
}

G4double ScintillatorConstruction::GetThickness(){

	// Returns Thickness in mm !

	return _ScintillatorThickness;
}

G4VPhysicalVolume* ScintillatorConstruction::Construct(){

	std::cout << "In ScintillatorConstruction::Construct()" << std::endl;

	// MATERIALS
	G4NistManager* NistManager = G4NistManager::Instance();

	// Experiment room gas
	G4Material* Air = NistManager->FindOrBuildMaterial("G4_AIR");
	//_SetOpticalPropertiesAir(Air);
	// Scintillator material
	G4Material* PVT = NistManager->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
	_SetOpticalPropertiesPVT(PVT);
	////////////////////////////////////////////////

	// GEOMETRY

	// Scintillator Slab Dimensions
	G4double Scintillator_x = 1.0*m;
	G4double Scintillator_y = 0.5*m;

	// Experiment Room Dimensions
	G4double ExpRoom_x = 3 * Scintillator_x;
	G4double ExpRoom_y = 2 * Scintillator_y;
	G4double ExpRoom_z = 10 * _ScintillatorThickness;

	// Experiment Room Implementation
	G4Box* ExpRoomSolid = new G4Box("ExperimentRoomSolid", ExpRoom_x, ExpRoom_y, ExpRoom_z);
	G4LogicalVolume* ExpRoomLogical = new G4LogicalVolume(ExpRoomSolid, Air, "ExperimentRoomLogical");
	G4VPhysicalVolume* ExpRoomPhysical = new G4PVPlacement(0, G4ThreeVector(), ExpRoomLogical, "ExperimentRoomPhysical", 0, false, 0);

	// Scintillator Slab Implementation
	G4Box* ScintillatorSolid = new G4Box("ScintillatorSolid", Scintillator_x, Scintillator_y, _ScintillatorThickness);
	G4LogicalVolume* ScintillatorLogical = new G4LogicalVolume(ScintillatorSolid, PVT, "ScintillatorLogical");
	G4VPhysicalVolume* ScintillatorPhysical = new G4PVPlacement(0, G4ThreeVector(), ScintillatorLogical, "ScintillatorPhysical", ExpRoomLogical, false, 0);


	return ExpRoomPhysical;
}

// Found these in the Geant4 WLS Example
void ScintillatorConstruction::_SetOpticalPropertiesAir(G4Material* Air){
	G4double photonEnergy[] =
	{2.00*eV,2.03*eV,2.06*eV,2.09*eV,2.12*eV,
	 2.15*eV,2.18*eV,2.21*eV,2.24*eV,2.27*eV,
	 2.30*eV,2.33*eV,2.36*eV,2.39*eV,2.42*eV,
	 2.45*eV,2.48*eV,2.51*eV,2.54*eV,2.57*eV,
	 2.60*eV,2.63*eV,2.66*eV,2.69*eV,2.72*eV,
	 2.75*eV,2.78*eV,2.81*eV,2.84*eV,2.87*eV,
	 2.90*eV,2.93*eV,2.96*eV,2.99*eV,3.02*eV,
	 3.05*eV,3.08*eV,3.11*eV,3.14*eV,3.17*eV,
	 3.20*eV,3.23*eV,3.26*eV,3.29*eV,3.32*eV,
	 3.35*eV,3.38*eV,3.41*eV,3.44*eV,3.47*eV};

	const G4int nEntries = sizeof(photonEnergy)/sizeof(G4double);

  //--------------------------------------------------
  // Air
  //--------------------------------------------------

	G4double refractiveIndex[] =
	{1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
	 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
     1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
     1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
     1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00};

	assert(sizeof(refractiveIndex) == sizeof(photonEnergy));

	G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
	mpt->AddProperty("RINDEX", photonEnergy, refractiveIndex, nEntries);

	Air->SetMaterialPropertiesTable(mpt);
}



void ScintillatorConstruction::_SetOpticalPropertiesPVT(G4Material* PVT){
	// USING DATASHEET OF BC-408 PVT Scintillator: http://www.phys.ufl.edu/courses/phy4803L/group_I/muon/bicron_bc400-416.pdf
	// And the values at: https://www-zeuthen.desy.de/lcdet/Feb_05_WS/talks/rd_lcdet_sim.pdf

	const G4int NUMENTRIES = 18;

	// Wavelengths and Rellightoutput read from: http://iopscience.iop.org/article/10.1209/0295-5075/95/22001/pdf
	G4double PhotonWavelengths_BC408[NUMENTRIES] =
	{350 * nm, 360 * nm, 370 * nm, 380 * nm, 390 * nm, 400 * nm, 410 * nm, 425 * nm, 440 * nm, 450 * nm,
	 460 * nm, 470 * nm, 480 * nm, 490 * nm, 500 * nm, 510 * nm, 520 * nm, 530 * nm};

	G4double RelLightOutput_BC408[NUMENTRIES] =
	{0.0, 0.02, 0.04, 0.08, 0.1, 0.2, 0.52, 1.0, 0.82, 0.58, 0.43, 0.26, 0.18, 0.12,
	 0.08, 0.04, 0.02, 0.0};

	G4double PhotonEnergy[NUMENTRIES];
	G4double RINDEX_Bc408[NUMENTRIES];
	G4double ABSORPTION_Bc408[NUMENTRIES];
	for (int i = 0; i < NUMENTRIES; i++){
		PhotonEnergy[i] = (h_Planck * c_light) / PhotonWavelengths_BC408[i];
		RINDEX_Bc408[i] = 1.58;
		ABSORPTION_Bc408[i] = 210 * cm;
	}
	/*
	G4double PhotonEnergy[NUMENTRIES] =
	{ 3.44*eV, 3.26*eV, 3.1*eV, 3.02*eV, 2.95*eV,
	2.92*eV, 2.82*eV, 2.76*eV, 2.7*eV, 2.58*eV,
	2.38*eV, 2.08*eV };*/

	/*G4double RINDEX_Bc408[NUMENTRIES]
	{ 1.58, 1.58, 1.58, 1.58, 1.58,
	1.58, 1.58, 1.58, 1.58, 1.58,
	1.58, 1.58 };

	G4double ABSORPTION_Bc408[NUMENTRIES] =
	{ 210*cm, 210*cm, 210*cm, 210*cm, 210*cm,
	210*cm, 210*cm, 210*cm, 210*cm, 210*cm,
	210*cm, 210*cm };*/

	/*G4double SCINTILLATION_Bc408[NUMENTRIES] =
	{ 0.04, 0.07, 0.20, 0.49, 0.84,
	1.00, 0.83, 0.55, 0.40, 0.17,
	0.03, 0.0 };*/

	G4MaterialPropertiesTable *Bc408_mt = new G4MaterialPropertiesTable();
	Bc408_mt->AddProperty("RINDEX", PhotonEnergy, RINDEX_Bc408, NUMENTRIES);
	Bc408_mt->AddProperty("ABSLENGTH", PhotonEnergy, ABSORPTION_Bc408, NUMENTRIES);

	//Scintillation yield of BC-408 found at http://cosmicray.umd.edu/atic/pub/ICRC_07/cream/icrc0620v4.pdf
	Bc408_mt->AddConstProperty("SCINTILLATIONYIELD", 100./MeV); // Real value is 10000/MeV
	Bc408_mt->AddConstProperty("RESOLUTIONSCALE", 1.0);

	// Organic Scintillator only have 1 decay time !
	// So by only defining FASTTIMECONSTANT and setting YIELDRATION = FASTTIMECONSTANT/TOTALDECAYTIME = 1.0
	// We have told Geant4 that there is only 1 decay time.
	Bc408_mt->AddProperty("FASTCOMPONENT", PhotonEnergy, RelLightOutput_BC408, NUMENTRIES);
	Bc408_mt->AddConstProperty("FASTTIMECONSTANT", 2.1*ns);
	Bc408_mt->AddConstProperty("YIELDRATIO", 1.);


	PVT->SetMaterialPropertiesTable(Bc408_mt);

}
