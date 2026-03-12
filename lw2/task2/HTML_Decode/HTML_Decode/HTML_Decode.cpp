#include <iostream>
#include <vector>
#include <algorithm>
#include "HTML_Decode.h"


std::string ParseCommand(const std::string& command)
{
    if (command == "&quot;") return "\"";
    if (command == "&apos;") return "'";
    if (command == "&lt;") return "<";
    if (command == "&gt;") return ">";
    if (command == "&amp;") return "&";
    return command;
}

std::string ProcessCommand(size_t& i, const std::string& html)
{
    std::string processedHtml;
    std::string command;

    while (html[i] != ';' && i < html.size() && html[i] != ' ') 
    {
        command += html[i];
        i++;
    }
    if (i < html.size())
    {
        command += html[i];
    }
    processedHtml += ParseCommand(command);

    return processedHtml;
}

std::string HtmlDecode(const std::string& html)
{
    std::string decodedHtml = "";
    for (size_t i = 0; i < html.size(); i++)
    {
        if (html[i] == '&')
        {
            decodedHtml += ProcessCommand(i, html);
        }
        else
        {
            decodedHtml += html[i];
        }
    }
    return decodedHtml;
}

