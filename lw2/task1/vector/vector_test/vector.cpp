#include <fstream>
#include <algorithm>
#include <iomanip>
#include <string>
#include <stdexcept>
#include "../vector/vector.h"

void ReadNumbers(std::istream& fin, std::vector<double>& vect)
{
	std::string number;
	while (fin >> number)
	{
		vect.push_back(std::stod(number));
	}
}

std::vector<double> ProcessNumbers(const std::vector<double>& vectIn)
{
	double minElement = *std::min_element(vectIn.begin(), vectIn.end());
	std::vector<double> vect = vectIn;

	for (size_t i = 0; i < vect.size(); i++)
	{
		
		if (std::isinf(vect[i]) || isnan(vect[i]))
		{
			throw std::invalid_argument("Invalid argument");
		}
		if (vect[i] > abs(std::numeric_limits<double>::lowest() / minElement) || vect[i] > abs(std::numeric_limits<double>::max() / minElement))
		{
			throw std::out_of_range("Out of range");
		}

		vect[i] *= minElement;
	}
	return vect;
}

void PrintSortedNumbers(std::ostream& fout, const std::vector<double>& vect)
{
	std::vector<double> sortedVect = vect;
	std::sort(sortedVect.begin(), sortedVect.end());

	fout << std::fixed << std::setprecision(3);
	for (size_t i = 0; i < sortedVect.size(); i++)
	{
		fout << sortedVect[i] << ' ';
	}
	fout << std::endl;
}