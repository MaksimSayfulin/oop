#include <iostream>
#include "Dict.h"
#include <cctype>
#include <locale>
#include <algorithm>
#include <Windows.h>
#include <boost/locale.hpp>

std::string Trim(const std::string& str)
{
	auto start = str.find_first_not_of(" \t\n\r\f\v");
	if (start == std::string::npos)
	{
		return "";
	}
	auto end = str.find_last_not_of(" \t\n\r\f\v");
	return str.substr(start, end - start + 1);
}

std::pair<Dict, Dict> ReadDict(std::istream& in)
{
	Dict dict, reversedDict;
	std::string eng, ru;
	while (std::getline(in, eng, '|') && std::getline(in, ru))
	{
		dict.insert({ eng, ru });
		reversedDict.insert({ ru, eng });
	}
	return { dict, reversedDict };
}

void SaveDict(std::ostream& out, const Dict& dict)
{
	for (auto [eng, ru] : dict)
	{
		out << eng << '|' << ru << std::endl;
	}
}

bool FindTranslation(const Dict& dict, const std::string& word, DictRange& range)
{
	range = dict.equal_range(word);
	if (range.first == dict.end())
	{
		return false;
	}
	return true;
}

bool AddTranslation(Dict& dict, const std::string& key, const std::string& value)
{
	if (key.empty())
	{
		return false;
	}
	dict.insert({ key, value });
	return true;
}

bool IsEnglishWord(const std::string& word)
{
	return word[0] >= 'a' && word[0] <= 'z';
}

void PrintTranslation(std::ostream& out, DictRange& range)
{
	for (auto it = range.first; it != range.second; it++)
	{
		out << it->second << ' ';
	}
	out << std::endl;
}

void ToLower(std::string& word)
{
	boost::locale::generator gen;
	std::locale loc = gen("");

	word = boost::locale::to_lower(word, loc);
}

void HandleTranslation(std::istream& in, const std::string& word, Dict& forward, Dict& backward)
{
	DictRange range;
	if (FindTranslation(forward, word, range))
	{
		PrintTranslation(std::cout, range);
	}
	else
	{
		std::cout << "Enter the translation of the " << word << std::endl;
		std::string translation;
		if (std::getline(in, translation))
		{
			translation = Trim(translation);
			ToLower(translation);
			if (!translation.empty())
			{
				AddTranslation(forward, word, translation);
				AddTranslation(backward, translation, word);
			}
			else
			{
				std::cout << "The word " << word << " has been ignored" << std::endl;
			}
		}
	}
}


void RunDict(std::istream& in, const std::string& filename)
{
	std::ifstream dictSource(filename);
	auto dicts = ReadDict(dictSource);
	Dict dictEnRu = dicts.first;
	Dict dictRuEn = dicts.second;
	size_t dictSize = dictEnRu.size();
	dictSource.close();

	std::string word;
	while (std::getline(in, word) && word != "...")
	{
		if (word == "")
		{
			continue;
		}
		DictRange range;
		word = Trim(word);
		ToLower(word);
		if (IsEnglishWord(word))
		{
			
			HandleTranslation(in, word, dictEnRu, dictRuEn);
		}
		else
		{
			HandleTranslation(in, word, dictRuEn, dictEnRu);
		}
	}

	if (dictSize != dictEnRu.size())
	{
		std::cout << "Changes have been made to the dictionary.Enter Y or y to save before exiting." << std::endl;
		char ch;
		in.get(ch);
		if (ch == 'Y' || ch == 'y')
		{
			std::ofstream dictDrain(filename);
			SaveDict(dictDrain, dictEnRu);
			std::cout << "Succesfully saved" << std::endl;
			dictDrain.close();
		}
	}
}

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	RunDict(std::cin, "dict.txt");
}