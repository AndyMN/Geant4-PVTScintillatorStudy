/*
	INPUT FILE PARSER FOR CONTROL OF PROGRAM

	1) READ A LINE AND INPUT IT INTO "get_property_value_pair". THIS WILL GIVE A PAIR WHERE THE FIRST IS
	THE VARIABLE NAME AND THE SECOND IS THE VALUE AS A STRING
	2) IF THE VALUE IS A SET OF VALUES THEY NEED TO BE SPLIT UP. THIS STRING THAT IS A SET OF VALUES WILL BE THE INPUT OF
	"get_values" WHICH WILL GIVE A VECTOR OF VECTORS OF STRINGS WHERE EACH VECTOR OF STRINGS CONTAINS A SET OF VALUES
	FOR INSTANCE: [[1, 2, 3], [4, 5]] THE SET OF VALUES [1, 2, 3] WILL BE PLACED IN A VECTOR OF STRINGS AND
	[4, 5] IN A DIFFERENT VECTOR OF STRINGS. THE STRINGS INSIDE THESE VECTORS WILL BE THE ELEMENTS AS STRINGS FOR INSTANCE
	"1" "2" "3" WILL BE THE ELEMENTS OF THE FIRST VECTOR OF STRINGS.
	3) IF THE VECTOR OF VECTORS OF STRINGS ARE SETS OF NUMERICAL VALUES WE USE THIS AS AN INPUT TO "get_number_values"
	WHICH WILL RETURN A VECTOR OF VECTORS OF DOUBLES.

*/
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include "FileParser.h"

template <typename T> void print_vector_of_vectors(std::vector<std::vector<T>>& vector_of_vectors);
template <typename T> void print_vector(std::vector<T>& vec);

std::vector<double> arange(double start, double stop, double step);

int main(int argc, char *argv[])
{
	std::string FileName = argv[1];
	FileParser MyFileParser = FileParser(FileName);

    std::vector<std::vector<double>> ERangeStart = MyFileParser.ERangeStart();
    std::vector<std::vector<double>> ERangeStop = MyFileParser.ERangeStop();
    std::vector<std::vector<double>> ERangeStep = MyFileParser.ERangeStep();
    std::vector<std::vector<double>> EDiscreteValues = MyFileParser.EDiscreteValues();


    std::vector<double> ParticleEnergies = MyFileParser.ParticleEnergies(ERangeStart, ERangeStop, ERangeStep, EDiscreteValues, 0);

	print_vector(ParticleEnergies);



    return 0;
}


template <typename T>
void print_vector_of_vectors(std::vector<std::vector<T>>& vector_of_vectors){
	/*
		Print all the values in the vector of vectors grouped per set
	*/

	typename std::vector<std::vector<T>>::iterator it;
	for (it = vector_of_vectors.begin(); it != vector_of_vectors.end(); ++it){
		std::cout << "SET START" << std::endl;
		typename std::vector<T>::iterator it2;
		for (it2 = (*it).begin(); it2 != (*it).end(); ++it2){
			std::cout << *it2 << std::endl;
		}
		std::cout << "SET STOP" << std::endl;
	}
}

template <typename T>
void print_vector(std::vector<T>& vec){

	typename std::vector<T>::iterator it;
	for (it = vec.begin(); it != vec.end(); ++it){
		std::cout << "|" <<*it << "|" <<std::endl;
	}
}

std::vector<double> arange(double start, double stop, double step){
	std::vector<double> created_vector;

	for (double i = start; i <= stop; i += step){
		created_vector.push_back(i);
	}
	return created_vector;
}

