#include "CSVReader.h"
#include <fstream>
#include <sstream>

void CSVReader::read(string file_path, vector<vector<int>>& map_structure)
{
	std::ifstream ifs(file_path);
	if (!ifs.is_open()) {
		return;
	}

	string line;
	int row = 0;
	while (std::getline(ifs,line))
	{
		map_structure.resize(row + 1);
		vector<string> map_info_list = split(line, ',');
		for (size_t i = 0; i < map_info_list.size(); i++)
		{
			map_structure[row].push_back(std::stoi(map_info_list.at(i)));
		}
		row++;
	}
}

vector<string> split(string& input, char delimiter)
{
	std::istringstream stream(input);
	string field;
	vector<string> result;
	while (std::getline(stream, field, delimiter))
	{
		result.push_back(field);
	}
	return result;
}
