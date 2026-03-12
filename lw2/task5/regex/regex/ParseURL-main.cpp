#include "ParseURL.h"
#include <string>
#include <iostream>

int main()
{
	std::string url;
	Protocol prot;
	int port;
	std::string host;
	std::string doc;
	while (std::getline(std::cin, url))
	{
		try
		{
			ParseURL(url, prot, port, host, doc);
			PrintURL(std::cout, url, host, prot, port, doc);
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
}