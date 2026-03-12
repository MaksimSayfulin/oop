#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>

const int UPPER_BOUND = 255;
const int LOWER_BOUND = 0;

enum class CoderMode
{
    Crypt,
    Decrypt,
    Uknown
};

uint8_t UnshakeBits(uint8_t buffer)
{
    uint8_t tempBuffer = 0;

    if (buffer & (1 << 7)) tempBuffer |= 1 << 4;
    if (buffer & (1 << 6)) tempBuffer |= 1 << 3;
    if (buffer & (1 << 5)) tempBuffer |= 1 << 7;
    if (buffer & (1 << 4)) tempBuffer |= 1 << 2;
    if (buffer & (1 << 3)) tempBuffer |= 1 << 1;
    if (buffer & (1 << 2)) tempBuffer |= 1 << 0;
    if (buffer & (1 << 1)) tempBuffer |= 1 << 6;
    if (buffer & (1 << 0)) tempBuffer |= 1 << 5;

    return tempBuffer;
}

uint8_t ShakeBits(uint8_t buffer)
{
    uint8_t tempBuffer = 0;

    if (buffer & (1 << 7)) tempBuffer |= 1 << 5;
    if (buffer & (1 << 6)) tempBuffer |= 1 << 1;
    if (buffer & (1 << 5)) tempBuffer |= 1 << 0;
    if (buffer & (1 << 4)) tempBuffer |= 1 << 7;
    if (buffer & (1 << 3)) tempBuffer |= 1 << 6;
    if (buffer & (1 << 2)) tempBuffer |= 1 << 4;
    if (buffer & (1 << 1)) tempBuffer |= 1 << 3;
    if (buffer & (1 << 0)) tempBuffer |= 1 << 2;

    return tempBuffer;
}

void CryptFile(std::istream& fin, std::ofstream& fout, uint8_t value, CoderMode mode)
{
    char ch;
    while (fin.get(ch))
    {
        uint8_t buffer = static_cast<uint8_t>(ch);
        
        if (mode == CoderMode::Crypt)
        {
            buffer ^= value;
            buffer = ShakeBits(buffer);
        }
        else
        {
            buffer = UnshakeBits(buffer);
            buffer ^= value;
        }
        fout.put(static_cast<char>(buffer));
    }
}

CoderMode ChooseMode(const std::string& modeStr)
{
    if (modeStr == "crypt") //func
    {
        return CoderMode::Crypt;
    }
    else if (modeStr == "decrypt")
    {
        return CoderMode::Decrypt;
    }
    return CoderMode::Uknown;
}

int main(int argCount, char* argVect[])
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    if (argCount != 5)
    {
        std::cerr << "Usage: crypt.exe crypt|decrypt <input file> <output file> <key>" << std::endl;
        return 1;
    }

    std::string modeStr = argVect[1];
    std::string inFilename = argVect[2], outFilename = argVect[3];
    uint8_t key;
    try 
    {
        int temp_key = std::stoi(argVect[4]);
        if (temp_key < LOWER_BOUND || temp_key > UPPER_BOUND) //const
        {
            std::cerr << "Key should be an integer in range [0, 255]" << std::endl;
            return 1;
        }
        key = static_cast<uint8_t>(temp_key);
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Key should be an integer in range [0, 255]" << std::endl;
        return 1;
    }
    CoderMode mode = ChooseMode(modeStr);
    
    if (mode == CoderMode::Uknown)
    {
        std::cerr << "Usage: crypt.exe crypt|decrypt <input file> <output file> <key> askjcbsakjcb" << std::endl;
        return 1;
    }
    std::ifstream fin(inFilename, std::ios::binary);
    std::ofstream fout(outFilename, std::ios::binary);

    if (!fin.is_open() || !fout.is_open())
    {
        std::cerr << "Can't open input or output file" << std::endl;
        return 1;
    }
    
    CryptFile(fin, fout, key, mode);
    return 0;
}

