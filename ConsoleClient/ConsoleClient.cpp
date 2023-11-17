#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <crow.h>
#include <cpr/cpr.h>

#define CRT_SECURE_NO_WARNINGS

struct Message
{
	std::string content;
	std::string author;
	time_t timestamp;

	Message() = default;

	Message(const std::string& content, const std::string& author, time_t timestamp) :
		content{ content },
		author{ author },
		timestamp{ timestamp }
	{
		std::cout << "Copy constructor\n";
	}

	Message(std::string&& content, std::string&& author, time_t timestamp) :
		content{ std::move(content) },
		author{ std::move(author) },
		timestamp{ timestamp }
	{
		/* EMPTY */
	}
};

bool listeningThreadGoing = true;
std::vector<Message> chat;

void listener()
{
	using namespace std::literals::chrono_literals;
	time_t lastTimestamp = 0;
	char* dateTime = new char[101];

	while (listeningThreadGoing)
	{
		try
		{
			auto response = cpr::Get(
				cpr::Url{ "http://localhost:18080/chat" },
				cpr::Parameters{ {"from", std::to_string(lastTimestamp)} }
			);
			auto messages = crow::json::load(response.text);
			for (int i = 0; i < messages.size(); i++)
			{
				Message message{ 
					std::move(messages[i]["content"].s()),
					std::move(messages[i]["author"].s()),
					static_cast<time_t>(messages[i]["timestamp"].i())
				};
				chat.push_back(message);
				ctime_s(dateTime, 100, &message.timestamp);
				dateTime[strlen(dateTime) - 1] = '\0';
				std::cout << std::format("[{} at {}]: {}\n",
					message.author,
					dateTime,
					message.content
				);
			}
			lastTimestamp = (chat.empty() ? 0 : chat[chat.size() - 1].timestamp + 1);
			std::this_thread::sleep_for(0.5s);
		}
		catch (std::exception exception)
		{
			std::cout << "[Listener] Error detected: " << exception.what() << "\nThe app will close after you enter any character\n";
			listeningThreadGoing = false;
		}
	}
}

int main()
{
	const std::string name{ "Macovei Alexandru" };
	std::thread listeningThread(listener);

	while (listeningThreadGoing)
	{
		std::string message;
		std::getline(std::cin, message);
		if (message == "q" || !listeningThreadGoing)
		{
			listeningThreadGoing = false;
			break;
		}

		try
		{
			auto response = cpr::Put(
				cpr::Url{ "http://localhost:18080/chat" },
				cpr::Payload{ {{"author", name}, {"content", message} } }
			);
		}
		catch (const std::exception& e)
		{
			listeningThreadGoing = false;
			std::cout << e.what();
		}
	}

	listeningThread.join();

	return 0;
}