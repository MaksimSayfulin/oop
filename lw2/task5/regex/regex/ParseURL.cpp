#include <regex>
#include <iostream>
#include "ParseURL.h"
#include <fstream>

const int MAX_PORT = 65535;
const int MIN_PORT = 1;
const int DEFAULT_HTTP_PORT = 80;
const int DEFAULT_HTTPS_PORT = 443;
const int DEFAULT_FTP_PORT = 21;

bool ParseProtocol(std::string& str, Protocol& prot)
{
    if (str == "http")
    {
        prot = Protocol::HTTP;
    }
    else if (str == "https") 
    {
        prot = Protocol::HTTPS;
    }
    else if (str == "ftp")
    {
        prot = Protocol::FTP;
    }
    else
    {
        return false;
    }
    return true;
}

bool ParsePort(std::string& str, int& port, const Protocol& protocol)
{
    if (!str.empty())
    {
        int p = std::stoi(str);
        if (p < MIN_PORT || p > MAX_PORT)
        {
            return false;
        }
        port = p;
    }
    else
    {
        switch (protocol)
        {
        case Protocol::HTTP:
            port = DEFAULT_HTTP_PORT;
            break;
        case Protocol::HTTPS:
            port = DEFAULT_HTTPS_PORT;
            break;
        case Protocol::FTP:
            port = DEFAULT_FTP_PORT;
            break;
        }
    }
    return true;
}

void ParseURL(std::string const& url, Protocol& protocol, int& port,
    std::string& host, std::string& document)
{
    const std::regex pattern(
        R"(^(https?|ftp)://([a-zA-Z0-9-]+(?:\.[a-zA-Z0-9-]+)*)(?::(\d{1,5}))?(?:/(.*))?$)",
        std::regex::icase
    );

    std::smatch matches;
    if (!std::regex_match(url, matches, pattern))
    {
        throw std::invalid_argument("Wrong URL format");
    }

    std::string prot = matches[1].str();
    for (auto& c : prot)
    {
        c = std::tolower(c);
    }

    
    if (!ParseProtocol(prot, protocol))
    {
        throw std::invalid_argument("Wrong protocol format");
    }

    host = matches[2].str();
    std::string portStr = matches[3].matched ? matches[3].str() : "";
    if (!ParsePort(portStr, port, protocol))
    {
        throw std::invalid_argument("Wrong port format");
    }
    document = matches[4].matched ? matches[4].str() : "";
}

void PrintURL(std::ostream& out, const std::string& url, const std::string& host, const Protocol& protocol, const int port, const std::string& document) 
{
    out << url << std::endl
        << "HOST: " << host << std::endl
        << "PORT: " << port << std::endl
        << "DOC: " << document << std::endl;
}
