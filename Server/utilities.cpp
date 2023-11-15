module utilities;

std::vector<std::string> utils::split(const std::string& str, const std::string& delim)
{
	std::vector<std::string> result;
	size_t startIndex = 0;

	for (size_t found = str.find(delim); found != std::string::npos; found = str.find(delim, startIndex))
	{
		result.emplace_back(str.begin() + startIndex, str.begin() + found);
		startIndex = found + delim.size();
	}
	if (startIndex != str.size())
		result.emplace_back(str.begin() + startIndex, str.end());
	return result;
}

std::string utils::decodeMessage(const std::string& message)
{
	std::string decodedMessage = "";
	for (int i = 0; i < message.length(); i++)
	{
		if (message[i] == '%')
		{
			std::string hex = message.substr(i + 1, 2);
			int decimal = std::stoi(hex, nullptr, 16);
			decodedMessage += static_cast<char>(decimal);
			i += 2;
		}
		else
		{
			decodedMessage += message[i];
		}
	}
	return decodedMessage;
}