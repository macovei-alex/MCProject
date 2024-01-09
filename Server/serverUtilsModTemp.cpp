#include "serverUtilsModTemp.h";

#include <unordered_map>
#include <regex>
#include <windows.h>
#include "..\UtilsDLL\utilsDLL.h"

std::string utils::DecodeMessage(const std::string& message)
{
	std::string decodedMessage;
	for (size_t i = 0; i < message.length(); i++)
	{
		if (message[i] == '%')
		{
			std::string hex = message.substr(i + 1, 2);
			uint8_t decimal = std::stoi(hex, nullptr, 16);
			decodedMessage += static_cast<char>(decimal);
			i += 2;
		}
		else
			decodedMessage += message[i];
	}
	return decodedMessage;
}

std::unordered_map<std::string, std::string> utils::ParseRequestBody(const std::string& requestBody)
{
	std::unordered_map<std::string, std::string> jsonMap;
	std::vector<std::string> params{ SplitToVector(requestBody, "&") };

	for (auto& informationExpression : params)
	{
		auto urlParamPair{ SplitToPair(informationExpression, "=") };
		jsonMap.emplace(std::move(urlParamPair));
	}

	return jsonMap;
}

/*
* The incoming string is expected to be in the following format:
* [{"key1":value1,"key2":value2,...},{"key1":value1,"key2":value2,...},...]
* 
* The values can have "" or not, but the keys must have " "
* The values can be int (if they dont have "") or string (if they have "")
*
* The { of a json object is pos1
* The : of a json object is pos2
* The , of a json object is pos3
* The } of a json object is pos4
*/

std::vector<std::unordered_map<std::string, std::variant<int64_t, std::string>>> utils::ListOfMapsFromJsonListStr(const std::string& jsonListStr)
{
	std::string cleanedStr;
	cleanedStr.reserve(jsonListStr.size());
	for (size_t i = 1; i < jsonListStr.size() - 1; i++)
	{
		if (!std::isspace(jsonListStr[i]))
			cleanedStr += jsonListStr[i];
	}

	std::vector<std::unordered_map<std::string, std::variant<int64_t, std::string>>> resultVector;
	std::unordered_map<std::string, std::variant<int64_t, std::string>> objectMap;
	std::string key, value;
	size_t pos1{ 0 }, pos2{ 0 }, pos3{ 0 }, pos4{ 0 };

	while ((pos1 = cleanedStr.find("{", pos1)) != std::string::npos)
	{
		pos4 = cleanedStr.find("}", pos1);

		for (/* empty */; pos1 < pos4; pos1 = pos3)
		{
			pos2 = cleanedStr.find(":", pos1);
			pos3 = cleanedStr.find(",", pos2);

			key = cleanedStr.substr(pos1 + 2, pos2 - pos1 - 3);
			if (pos3 < pos4)
				value = cleanedStr.substr(pos2 + 1, pos3 - pos2 - 1);
			else
				value = cleanedStr.substr(pos2 + 1, pos4 - pos2 - 1);

			if (value[0] != '"')
			{
				int64_t intValue = std::stoi(value);
				objectMap[key] = intValue;
			}
			else
				objectMap[key] = value.substr(1, value.size() - 2);
		}
		resultVector.emplace_back(std::move(objectMap));
		objectMap.clear();
		pos1 = pos4;
	}

	return resultVector;
}

// Functie chemata pentru a putea folosi fisierele de resurse din folderul Project/Server
// chiar daca executabilul a fost rulat din Project/Server/x64/[Debug|Release]
void utils::NavigateToProjectDirectory()
{
	const DWORD buffer_size = MAX_PATH;
	TCHAR buffer[buffer_size];

	DWORD length = GetCurrentDirectoryW(buffer_size, buffer);
	std::wstring currentDirectory{ buffer, length };

	// std::wcout << currentDirectory << std::endl;

	size_t x64pos;
	if ((x64pos = currentDirectory.find(L"x64")) != std::wstring::npos)
	{
		currentDirectory = currentDirectory.substr(0, x64pos);
		SetCurrentDirectory((currentDirectory + L"\\Server").c_str());
	}

	// std::wcout << currentDirectory << std::endl;
}
