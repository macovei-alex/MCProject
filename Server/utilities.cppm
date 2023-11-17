export module utilities;

import <vector>;
import <string>;

namespace utils 
{
	export std::vector<std::string> splitToVec(const std::string& str, const std::string& delim);
	export std::pair<std::string, std::string> splitToPair(const std::string& str, const std::string& delim);
	export std::string decodeMessage(const std::string& message);
}