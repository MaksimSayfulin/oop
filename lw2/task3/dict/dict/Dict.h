#pragma once
#include <fstream>
#include <unordered_map>
#include <string>

using Dict = std::unordered_multimap<std::string, std::string>;
using DictRange = std::pair<Dict::const_iterator, Dict::const_iterator>;

void RunDict(std::istream& in, const std::string& filename);
