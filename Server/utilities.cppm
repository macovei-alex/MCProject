export module utilities;

import <vector>;
import <string>;

namespace utils 
{
	export std::vector<std::string> split(const std::string& str, const std::string& delim);
	export std::string decodeMessage(const std::string& message);
}