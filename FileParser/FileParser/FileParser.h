#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <string>
#include <vector>
#include <map>

class FileParser{

public:
	FileParser(std::string FileName);

	std::string DataSetPostFix();
	std::string DataFileDirectory();
	std::string DataFileOption();
	std::vector<std::vector<double>> ERangeStart();
	std::vector<std::vector<double>> ERangeStop();
	std::vector<std::vector<double>> ERangeStep();
	std::vector<double> SlabThickness();
	std::vector<double> SecondaryParticles();
	std::vector<double> StoppingPower();
	std::vector<double> LandauDistribution();
	std::vector<double> PhotonWavelengths();
	std::vector<double> OptPhotonTimeAnalysis();
	std::vector<double> OptPhotonAmount();
	std::vector<double> OptPhotonAmountdE();
	std::vector<double> OptPhotonPoisson();
	std::vector<double> OptProcessesdE();
	std::vector<double> OptProcessesE();
	std::vector<double> OptParentNamedE();
	std::vector<double> OptParentNameE();
	std::vector<std::vector<double>> EDiscreteValues();
	std::vector<std::string> ParticleTypes();
	int NumEvents();
	int GenerateDataFiles();
	int MakePlots();
	int SaveHistograms();
	int AnalyzeData();
	int UseOpticalPhysics();


	std::vector<double> ParticleEnergies(std::vector<std::vector<double>> ERangeStart, std::vector<std::vector<double>> ERangeStop, std::vector<std::vector<double>> ERangeStep, std::vector<std::vector<double>> EDiscreteValues, int particle_index);

	std::map<std::string, bool> AnalysisMap(std::string AnalysisName);


private:
	std::string _DataSetPostFix;
	std::string _DataFileDirectory;
	std::string _DataFileOption;
	std::vector<std::vector<double>> _ERangeStart;
	std::vector<std::vector<double>> _ERangeStop;
	std::vector<std::vector<double>> _ERangeStep;
	std::vector<double> _SlabThickness;
	std::vector<double> _SecondaryParticles;
	std::vector<double> _StoppingPower;
	std::vector<double> _LandauDistribution;
	std::vector<double> _PhotonWavelengths;
	std::vector<double> _OptPhotonTimeAnalysis;
	std::vector<double> _OptPhotonAmount;
	std::vector<double> _OptPhotonAmountdE;
	std::vector<double> _OptPhotonPoisson;
	std::vector<double> _OptProcessesdE;
	std::vector<double> _OptProcessesE;
	std::vector<double> _OptParentNamedE;
	std::vector<double> _OptParentNameE;
	std::vector<std::vector<double>> _EDiscreteValues;
	std::vector<std::string> _ParticleTypes;
	int _NumEvents;
	int _GenerateDataFiles;
	int _MakePlots;
	int _AnalyzeData;
	int _SaveHistograms;
	int _UseOpticalPhysics;


	void _process_file(std::string FileName);
	std::pair<std::string, std::string> _get_property_value_pair (std::string& dataline);

	std::vector<std::string> _extract_str_values_from_array_string(std::string& set_of_values);
	std::vector<double> _extract_num_values_from_array_string(std::string& set_of_values);
	std::vector<std::vector<double>> _extract_vectors_from_array_string (std::string& set_of_arrays);

	int _number_of_characters(std::string& string_to_count_in, char char_to_count);
	std::string _remove_brackets(std::string& array_string);

	std::vector<double> _arange(double start, double stop, double step);




};

#endif
