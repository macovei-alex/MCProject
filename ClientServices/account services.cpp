#include "services.h"

bool services::SignIn(const std::string& username, const std::string& password, std::ostream& outStream, std::ostream& errStream)
{
	try
	{
		std::stringstream url;
		url << literals::routes::baseAddress << literals::routes::account::signIn;

		auto response = cpr::Get(
			cpr::Url{ url.str() },
			cpr::Parameters{
				{literals::jsonKeys::account::username, username},
				{literals::jsonKeys::account::password, password}
			});

		if (response.status_code != 200 && response.status_code != 201)
		{
			if (!response.reason.empty())
				throw std::exception(response.reason.c_str());
			else
				throw std::exception("Server didn't return an error explanation");
		}

		outStream << std::format("[Sign In] {}\n", response.text);
		return true;
	}
	catch (const std::exception& exception)
	{
		errStream << "[Sign In]: " << exception.what() << '\n';
		return false;
	}
}

bool services::SignUp(const std::string& username, const std::string& password, std::ostream& outStream, std::ostream& errStream)
{
	try
	{
		std::stringstream url;
		url << literals::routes::baseAddress << literals::routes::account::singUp;

		auto response = cpr::Post(
			cpr::Url{ url.str() },
			cpr::Payload{
				{literals::jsonKeys::account::username, username},
				{literals::jsonKeys::account::password, password}
			});

		if (response.status_code != 200 && response.status_code != 201)
		{
			if (!response.reason.empty())
				throw std::exception(response.reason.c_str());
			else
				throw std::exception("Server didn't return an error explanation");
		}

		outStream << std::format("[Sign Up]: Account created with username < {} >\n", username);
		outStream << std::format("[Sign Up]: Signed up as < {} >\n", username);
		return true;
	}
	catch (const std::exception& exception)
	{
		errStream << "[Sign Up]: " << exception.what() << '\n';
		return false;
	}
}

bool services::SignOut(const std::string& username, std::ostream& outStream, std::ostream& errStream)
{
	try
	{
		std::stringstream url;
		url << literals::routes::baseAddress << literals::routes::account::signOut;

		auto response = cpr::Put(
			cpr::Url{ url.str() },
			cpr::Payload{ {literals::jsonKeys::account::username, username} });

		if (response.status_code != 200 && response.status_code != 201)
		{
			if (!response.reason.empty())
				throw std::exception(response.reason.c_str());
			else
				throw std::exception("Server didn't return an error explanation");
		}

		outStream << std::format("Signed out from < {} >\n", username);
		return true;
	}
	catch (const std::exception& exception)
	{
		errStream << "[Sign Out]: " << exception.what() << '\n';
		return false;
	}
}