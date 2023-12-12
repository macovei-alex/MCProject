#include "serverUtils.h"

#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>

std::string utils::getHash(const std::string& input)
{
	std::string hash;
	CryptoPP::SHA256 sha256;
	CryptoPP::StringSource(input, true, new CryptoPP::HashFilter(sha256, new CryptoPP::HexEncoder(new CryptoPP::StringSink(hash))));
	return hash;
}

// Functie chemata pentru a putea folosi fisierele de resurse din folderul Project/Server
// chiar daca executabilula fost rulat din Project/Server/x64/Debug
void utils::NavigateToProjectDirectory()
{
	const DWORD buffer_size = MAX_PATH;
	TCHAR buffer[buffer_size];

	DWORD length = GetCurrentDirectory(buffer_size, buffer);
	std::wstring currentDirectory{ buffer, length };

	size_t x64pos;
	if ((x64pos = currentDirectory.find(L"x64")) != std::wstring::npos)
	{
		currentDirectory = currentDirectory.substr(0, x64pos);
		SetCurrentDirectory((currentDirectory + L"\\Server").c_str());
	}
}