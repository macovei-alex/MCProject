//export module serverUtilsMod;
//
//import <vector>;
//import <string>;
//import <chrono>;
//import <map>;
//
//namespace utils
//{
//	struct Message
//	{
//		std::string content;
//		std::string author;
//		uint64_t timeMilliseconds;
//
//		Message() noexcept = default;
//		Message(std::string&& content, std::string&& author, uint64_t milliseconds) noexcept;
//		Message(const Message& other) noexcept = default;
//		Message(Message&& other) noexcept = default;
//		Message& operator=(const Message& other) noexcept = default;
//		Message& operator=(Message&& other) noexcept = default;
//	};
//
//	std::vector<std::string> SplitToVec(const std::string& str, const std::string& delim);
//	std::pair<std::string, std::string> SplitToPair(const std::string& str, const std::string& delim);
//	std::string DecodeMessage(const std::string& message);
//	uint64_t DateTimeAsInteger(std::chrono::system_clock::time_point dateTime = std::chrono::system_clock::now());
//	std::map<std::string, std::string> ParseRequestBody(const std::string& requestBody);
//}