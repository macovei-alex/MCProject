#include "utilities.h"

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