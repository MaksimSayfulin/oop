#pragma once
#include <string>

enum class Protocol
{
    HTTP,
    HTTPS,
    FTP
};

void ParseURL(std::string const& url, Protocol& protocol, int& port, std::string& host, std::string& document);
void PrintURL(std::ostream& out, const std::string& url, const std::string& host, const Protocol& protocol, const int port, const std::string& document);