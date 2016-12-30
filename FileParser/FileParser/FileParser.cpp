#include "FileParser.h"

#include <fstream>
#include <iostream>
#include <algorithm>

FileParser::FileParser(std::string FileName): _NumEvents(0), _GenerateDataFiles(0), _AnalyzeData(0), _SaveHistograms(0),
											  _UseOpticalPhysics(0){
	_process_file(FileName);
}

void FileParser::_process_file(std::string FileName){

	std::ifstream File;
	File.open(FileName.c_str());

	std::string line;
	if (File.is_open()){
		while(std::getline(File, line)){
			std::string first_char = line.substr(0, 1);
			std::size_t last_space_index = line.find_last_not_of(" ");
			std::size_t equal_sign_index = line.find("=");
			std::size_t line_delimiter_index = line.find(";");
			// # Is our comment charachter
			// If last_space_index == std::string::npos this means the entire line is blank !
			// If equal_sign_index == std::string::npos this means that there is no equal sign in the line and thus invalid
			// If line_delimiter_index == std::string::npos this means that there is no valid end of line charachter and thus the line is invalid
			if (first_char.compare("#") != 0 && last_space_index != std::string::npos && equal_sign_index != std::string::npos && line_delimiter_index != std::string::npos){
				std::pair<std::string, std::string> property_and_value = _get_property_value_pair(line);

				std::string VariableName = property_and_value.first;
				if (VariableName == "DataSetPostFix"){
					_DataSetPostFix = property_and_value.second;
				}else if (VariableName == "DataFileDirectory"){
					_DataFileDirectory = property_and_value.second;
				}else if (VariableName == "DataFileOption"){
					_DataFileOption = property_and_value.second;
				}else if (VariableName == "ERangeStart"){
					_ERangeStart = _extract_vectors_from_array_string(property_and_value.second);
				}else if (VariableName == "ERangeStop"){
					_ERangeStop = _extract_vectors_from_array_string(property_and_value.second);
				}else if (VariableName == "ERangeStep"){
					_ERangeStep = _extract_vectors_from_array_string(property_and_value.second);
				}else if (VariableName == "ParticleTypes"){
					_ParticleTypes = _extract_str_values_from_array_string(property_and_value.second);
				}else if (VariableName == "NumEvents"){
					_NumEvents = std::stoi(property_and_value.second);
				}else if (VariableName == "GenerateDataFiles"){
					_GenerateDataFiles = std::stoi(property_and_value.second);
				}else if (VariableName == "EDiscreteValues"){
					_EDiscreteValues = _extract_vectors_from_array_string(property_and_value.second);
				}else if (VariableName == "SlabThickness"){
					_SlabThickness = _extract_num_values_from_array_string(property_and_value.second);
				}else if (VariableName == "MakePlots"){
					_MakePlots = std::stoi(property_and_value.second);
				}else if (VariableName == "SecondaryParticles"){
					_SecondaryParticles = _extract_num_values_from_array_string(property_and_value.second);
				}else if (VariableName == "AnalyzeData"){
					_AnalyzeData = std::stoi(property_and_value.second);
				}else if (VariableName == "SaveHistograms"){
					_SaveHistograms = std::stoi(property_and_value.second);
				}else if (VariableName == "StoppingPower"){
					_StoppingPower = _extract_num_values_from_array_string(property_and_value.second);
				}else if (VariableName == "LandauDistribution"){
					_LandauDistribution = _extract_num_values_from_array_string(property_and_value.second);
				}else if (VariableName == "PhotonWavelengths"){
					_PhotonWavelengths = _extract_num_values_from_array_string(property_and_value.second);
				}else if (VariableName == "UseOpticalPhysics"){
					_UseOpticalPhysics = std::stoi(property_and_value.second);
				}else if (VariableName == "OptPhotonTimeAnalysis"){
					_OptPhotonTimeAnalysis = _extract_num_values_from_array_string(property_and_value.second);
				}else if (VariableName == "OptPhotonAmount"){
					_OptPhotonAmount = _extract_num_values_from_array_string(property_and_value.second);
				}else if (VariableName == "OptPhotonAmountdE"){
					_OptPhotonAmountdE = _extract_num_values_from_array_string(property_and_value.second);
				}else if (VariableName == "OptPhotonPoisson"){
					_OptPhotonPoisson = _extract_num_values_from_array_string(property_and_value.second);
				}else if (VariableName == "OptProcessesdE"){
					_OptProcessesdE = _extract_num_values_from_array_string(property_and_value.second);
				}else if (VariableName == "OptProcessesE"){
					_OptProcessesE = _extract_num_values_from_array_string(property_and_value.second);
				}else if (VariableName == "OptParentNamedE"){
					_OptParentNamedE = _extract_num_values_from_array_string(property_and_value.second);
				}else if (VariableName == "OptParentNameE"){
					_OptParentNameE = _extract_num_values_from_array_string(property_and_value.second);
				}else{
					std::cerr << "VARIABLE NAME NOT RECOGNIZED ! " << VariableName << std::endl;
				}

			}
		}
	}
	File.close();
}



int FileParser::_number_of_characters(std::string& string_to_count_in, char char_to_count){
	/*
		COUNTS THE NUMBER OF TIMES char_to_count EXISTS INSIDE OF string_to_count_in
	*/
	int num_chars = 0;
	for (unsigned int i = 0; i < string_to_count_in.size(); i++){
		if (string_to_count_in[i] == char_to_count){
			num_chars++;
		}
	}

	return num_chars;
}

std::string FileParser::_remove_brackets(std::string& array_string){
	std::string processed_string = array_string;

	int number_of_left_brackets = _number_of_characters(processed_string, '[');
	int number_of_right_brackets = _number_of_characters(processed_string, ']');

	if (number_of_left_brackets != number_of_right_brackets){
		std::cerr << "Bracket amount mismatch !!! ERRORS AHOY" << std::endl;
	}else if (number_of_left_brackets >= 1){
		processed_string.erase(0, 1);
		processed_string.erase(processed_string.size() - 1);
	}

	return processed_string;

}

std::vector<std::string> FileParser::_extract_str_values_from_array_string(std::string& set_of_values){
	/*
		This function expects an input string like [4, 2, 1] and will return a vector of strings where
		each string is an element from this string array: "4" "2" "1".
	*/
	// Remove outside brackets [ ]
	std::string processed_values = _remove_brackets(set_of_values);


	std::vector<std::string> array_of_values;
	if (processed_values.compare("empty") != 0){
	// Find out how many values are inside of the array
		int number_of_commas = _number_of_characters(processed_values, ',');

		for (int i = 0; i <= number_of_commas; i++){
			int index_of_comma = processed_values.find(",");
			array_of_values.push_back(processed_values.substr(0, index_of_comma));
			processed_values.erase(0, index_of_comma + 1);
		}
	}else{
		array_of_values.push_back("empty");
	}
	return array_of_values;
}

std::vector<double> FileParser::_extract_num_values_from_array_string(std::string& set_of_values){
	/*
		This function expects an input string like [4, 2, 1] and will return a vector of strings where
		each string is an element from this string array: "4" "2" "1".
	*/
	// Remove outside brackets [ ]
	std::string processed_values = _remove_brackets(set_of_values);


	std::vector<double> array_of_values;
	if (processed_values.compare("empty") != 0){
	// Find out how many values are inside of the array
		int number_of_commas = _number_of_characters(processed_values, ',');


		for (int i = 0; i <= number_of_commas; i++){
			int index_of_comma = processed_values.find(",");
			std::string number = processed_values.substr(0, index_of_comma);
			array_of_values.push_back(std::stod(number));
			processed_values.erase(0, index_of_comma + 1);
		}
	}else{
		array_of_values.push_back(0);
	}
	return array_of_values;
}

std::vector<std::vector<double>> FileParser::_extract_vectors_from_array_string (std::string& set_of_arrays){
	std::vector<std::vector<double>> collection_of_vectors;

	// Remove outside brackets [1, 2, 3] becomes 1, 2, 3 and [[1,2],[4,5]] becomes [1,2],[4,5]
	std::string processed_set_of_arrays = _remove_brackets(set_of_arrays);

	int number_of_arrays = _number_of_characters(processed_set_of_arrays, ']');
	if (number_of_arrays == 0){ // [1, 2, 3] will have 0 arrays
		std::vector<double> values_in_array = _extract_num_values_from_array_string(processed_set_of_arrays);
		for (unsigned int i = 0; i < values_in_array.size(); i++){
			collection_of_vectors.push_back(std::vector<double>(1, values_in_array[i]));
		}
	}else if (number_of_arrays > 0){

		int index_of_left_bracket = 0;
		int index_of_right_bracket = 0;
		int index_of_left_commabracket = 0;

		for (int i = 0; i < number_of_arrays; i++){
			index_of_left_bracket = processed_set_of_arrays.find("[");
			index_of_right_bracket = processed_set_of_arrays.find("]");
			index_of_left_commabracket = processed_set_of_arrays.find(",[");

			// Extract the individual arrays inside of set_of_values.
			// if processed_value == [1,2,3], [2, 6], [], [1]
			// then set_of_values will == 1,2,3  and 2,6  and "empty"(the word empty)  and 1
			std::string set_of_values;
			if (index_of_left_bracket + 1 != index_of_right_bracket){
				set_of_values = processed_set_of_arrays.substr(index_of_left_bracket + 1, index_of_right_bracket - 1);
			}else{
				set_of_values = "empty";
			}
			// Remove the previous extracted set_of_values from the processed_value line
			processed_set_of_arrays.erase(index_of_left_bracket, index_of_left_commabracket + 1);
			// Split a set_of_values into elements and push each element to a vector array_of_values
			// 2,3,4 will be a vector of strings of "2" "3" and "4".
			std::vector<double> array_of_values = _extract_num_values_from_array_string(set_of_values);
			collection_of_vectors.push_back(array_of_values);
		}


	}
	return collection_of_vectors;
}


std::pair<std::string, std::string> FileParser::_get_property_value_pair (std::string& inputline){
	/*
		The input is a line like:  axaxa = popop;  # COMMENTS
		This function will process this line and give a tuple (axaxa, popop)
	*/
	std::pair<std::string, std::string> property_and_value;

	// Get only items before the delimiter
	int line_delimiter_index = inputline.find(";"); // use ; as a command delimiter like C++
	std::string data_line = inputline.substr(0, line_delimiter_index);

	// Clear all spaces from string
	std::string::iterator end_pos = std::remove(data_line.begin(), data_line.end(), ' ');
	data_line.erase(end_pos, data_line.end());


	// Find equal sign and fetch left and right side
	unsigned int equal_sign_index = data_line.find("=");

	if (equal_sign_index != std::string::npos){
		std::string left_of_equals = data_line.substr(0, equal_sign_index);
		std::string right_of_equals = data_line.substr(equal_sign_index + 1, std::string::npos);


		property_and_value.first = left_of_equals;
		property_and_value.second = right_of_equals;
	}else{
		std::cerr << "No equals sign detected ! Check input file !" << std::endl;
	}


	return property_and_value;
}

std::vector<double> FileParser::ParticleEnergies(std::vector<std::vector<double>> ERangeStart, std::vector<std::vector<double>> ERangeStop, std::vector<std::vector<double>> ERangeStep, std::vector<std::vector<double>> EDiscreteValues, int particle_index){

	// Create the values of energy defined by the range in the input file.
	std::vector<double> EnergiesFromRange;

	if (ERangeStart.size() > particle_index && ERangeStop.size() > particle_index && ERangeStep.size() > particle_index){
		std::vector<double> ERangeStartValues = ERangeStart[particle_index];
		std::vector<double> ERangeStopValues = ERangeStop[particle_index];
		std::vector<double> ERangeStepValues = ERangeStep[particle_index];

		if (ERangeStartValues.size() != ERangeStopValues.size() || ERangeStartValues.size() != ERangeStepValues.size()){
			std::cerr << "Range values dimensions mismatch !" << std::endl;
			return EnergiesFromRange;
		}

		for (int i = 0; i < ERangeStartValues.size(); i++){
			if (ERangeStartValues[i] > 0.0 && ERangeStepValues[i] > 0.0 && ERangeStopValues[i] > 0.0){
				std::vector<double> SetOfEnergiesFromRange = _arange(ERangeStartValues[i], ERangeStopValues[i], ERangeStepValues[i]);
				EnergiesFromRange.insert(EnergiesFromRange.end(), SetOfEnergiesFromRange.begin(), SetOfEnergiesFromRange.end());
			}
		}
	}

	// Get discrete energy values in a vector
	std::vector<double> EnergiesFromDiscretes;
	if (EDiscreteValues.size() > particle_index){
		EnergiesFromDiscretes = EDiscreteValues[particle_index];
		// Remove all 0's
		EnergiesFromDiscretes.erase(std::remove(EnergiesFromDiscretes.begin(), EnergiesFromDiscretes.end(), 0), EnergiesFromDiscretes.end());
	}

	// Combine Discrete and Range energy values for a total energy vector.
	std::vector<double> ParticleEnergies = EnergiesFromRange;
	ParticleEnergies.insert(ParticleEnergies.end(), EnergiesFromDiscretes.begin(), EnergiesFromDiscretes.end());

	sort(ParticleEnergies.begin(), ParticleEnergies.end());
	ParticleEnergies.erase(unique(ParticleEnergies.begin(), ParticleEnergies.end()), ParticleEnergies.end());

	return ParticleEnergies;
}

std::vector<double> FileParser::_arange(double start, double stop, double step){
	std::vector<double> created_vector;

	for (double i = start; i <= stop; i += step){
		created_vector.push_back(i);
	}
	return created_vector;
}

std::string FileParser::DataSetPostFix(){
	return _DataSetPostFix;
}

std::string FileParser::DataFileDirectory(){
	return _DataFileDirectory;
}

std::string FileParser::DataFileOption(){
	return _DataFileOption;
}

std::vector<std::vector<double>> FileParser::ERangeStart(){
	return _ERangeStart;
}

std::vector<std::vector<double>> FileParser::ERangeStop(){
	return _ERangeStop;
}

std::vector<std::vector<double>> FileParser::ERangeStep(){
	return _ERangeStep;
}

std::vector<std::vector<double>> FileParser::EDiscreteValues(){
	return _EDiscreteValues;
}

std::vector<std::string> FileParser::ParticleTypes(){
	return _ParticleTypes;
}

int FileParser::NumEvents(){
	return _NumEvents;
}

int FileParser::GenerateDataFiles(){
	return _GenerateDataFiles;
}

std::vector<double> FileParser::SlabThickness(){
	return _SlabThickness;
}

int FileParser::MakePlots(){
	return _MakePlots;
}

std::vector<double> FileParser::SecondaryParticles(){
	return _SecondaryParticles;
}

int FileParser::AnalyzeData(){
	return _AnalyzeData;
}

int FileParser::SaveHistograms(){
	return _SaveHistograms;
}

std::vector<double> FileParser::StoppingPower(){
	return _StoppingPower;
}

std::vector<double> FileParser::LandauDistribution(){
	return _LandauDistribution;
}

std::vector<double> FileParser::PhotonWavelengths(){
	return _PhotonWavelengths;
}

int FileParser::UseOpticalPhysics(){
	return _UseOpticalPhysics;
}

std::vector<double> FileParser::OptPhotonTimeAnalysis(){
	return _OptPhotonTimeAnalysis;
}

std::vector<double> FileParser::OptPhotonAmount(){
	return _OptPhotonAmount;
}

std::vector<double> FileParser::OptPhotonAmountdE(){
	return _OptPhotonAmountdE;
}

std::vector<double> FileParser::OptPhotonPoisson(){
	return _OptPhotonPoisson;
}

std::vector<double> FileParser::OptProcessesdE(){
	return _OptProcessesdE;
}

std::vector<double> FileParser::OptProcessesE(){
	return _OptProcessesE;
}

std::vector<double> FileParser::OptParentNamedE(){
	return _OptParentNamedE;
}

std::vector<double> FileParser::OptParentNameE(){
	return _OptParentNameE;
}

std::map<std::string, bool> FileParser::AnalysisMap(std::string AnalysisName){
	std::map<std::string, bool> analysis_map;

	if (AnalysisName == "StoppingPower"){
		for (int i = 0; i < _ParticleTypes.size(); i++){
			if (_StoppingPower.size() > i){
				analysis_map.insert(std::pair<std::string, bool>(_ParticleTypes[i], _StoppingPower[i]));
			}
		}
	}else if (AnalysisName == "LandauDistribution"){
		for (int i = 0; i < _ParticleTypes.size(); i++){
			if (_LandauDistribution.size() > i){
				analysis_map.insert(std::pair<std::string, bool>(_ParticleTypes[i], _LandauDistribution[i]));
			}
		}
	}else if (AnalysisName == "PhotonWavelengths"){
		for (int i = 0; i < _PhotonWavelengths.size(); i++){
			if (_PhotonWavelengths.size() > i){
				analysis_map.insert(std::pair<std::string, bool>(_ParticleTypes[i], _PhotonWavelengths[i]));
			}
		}
	}else if (AnalysisName == "OptPhotonTimeAnalysis"){
		for (int i = 0; i < _OptPhotonTimeAnalysis.size(); i++){
			if (_OptPhotonTimeAnalysis.size() > i){
				analysis_map.insert(std::pair<std::string, bool>(_ParticleTypes[i], _OptPhotonTimeAnalysis[i]));
			}
		}
	}else if (AnalysisName == "OptPhotonAmount"){
		for (int i = 0; i < _OptPhotonAmount.size(); i++){
			if (_OptPhotonAmount.size() > i){
				analysis_map.insert(std::pair<std::string, bool>(_ParticleTypes[i], _OptPhotonAmount[i]));
			}
		}
	}else if (AnalysisName == "OptPhotonAmountdE"){
		for (int i = 0; i < _OptPhotonAmountdE.size(); i++){
			if (_OptPhotonAmountdE.size() > i){
				analysis_map.insert(std::pair<std::string, bool>(_ParticleTypes[i], _OptPhotonAmountdE[i]));
			}
		}
	}else if (AnalysisName == "SecondaryParticles"){
		for (int i = 0; i < _SecondaryParticles.size(); i++){
			if (_SecondaryParticles.size() > i){
				analysis_map.insert(std::pair<std::string, bool>(_ParticleTypes[i], _SecondaryParticles[i]));
			}
		}
	}else if (AnalysisName == "OptProcessesdE"){
		for (int i = 0; i < _OptProcessesdE.size(); i++){
			if (_OptProcessesdE.size() > i){
				analysis_map.insert(std::pair<std::string, bool>(_ParticleTypes[i], _OptProcessesdE[i]));
			}
		}
	}else if (AnalysisName == "OptProcessesE"){
		for (int i = 0; i < _OptProcessesE.size(); i++){
			if (_OptProcessesE.size() > i){
				analysis_map.insert(std::pair<std::string, bool>(_ParticleTypes[i], _OptProcessesE[i]));
			}
		}
	}else if (AnalysisName == "OptParentNamedE"){
		for (int i = 0; i < _OptParentNamedE.size(); i++){
			if (_OptParentNamedE.size() > i){
				analysis_map.insert(std::pair<std::string, bool>(_ParticleTypes[i], _OptParentNamedE[i]));
			}
		}
	}else if (AnalysisName == "OptParentNameE"){
		for (int i = 0; i < _OptParentNameE.size(); i++){
			if (_OptParentNameE.size() > i){
				analysis_map.insert(std::pair<std::string, bool>(_ParticleTypes[i], _OptParentNameE[i]));
			}
		}
	}else{
		std::cerr << "Analysis name not recognized ! " << std::endl;
	}

	return analysis_map;
}
