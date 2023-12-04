#pragma once

namespace literals
{
	constexpr char emptyCString[] = "";
	const std::string emptyString{ "" };
	constexpr char error[] = "error";
	constexpr char intMinStr[] = "-2147483648";

	namespace jsonKeys
	{
		namespace message
		{
			constexpr char author[] = "author";
			constexpr char content[] = "content";
			constexpr char timestamp[] = "timestamp";
		}

		namespace room 
		{
			constexpr char ID[] = "roomID";
		}

		namespace account
		{
			constexpr char username[] = "username";
			constexpr char password[] = "password";
		}

		namespace draw
		{
			constexpr char pointX[] = "x";
			constexpr char pointY[] = "y";
			constexpr char color[] = "color";
			constexpr char timestamp[] = "timestamp";
		}
	}

	namespace routes
	{
		constexpr char baseAddress[] = "http://localhost:18080";
		constexpr char test[] = "/test";

		namespace game 
		{
			constexpr char chat[] = "/game/chat";
			constexpr char chatParam[] = "/game/chat/<int>";
		}

		namespace room
		{
			constexpr char create[] = "/room/create";
			constexpr char connect[] = "/room/connect";
			constexpr char connectParam[] = "/room/connect/<int>";
			constexpr char disconnect[] = "/room/disconnect";
			constexpr char disconnectParam[] = "/room/disconnect/<int>";
		}

		namespace sign
		{
			constexpr char in[] = "/sign/in";
			constexpr char up[] = "/sign/up";
			constexpr char out[] = "/sign/out";
		}

		namespace draw
		{
			constexpr char getUpdates[] = "/draw/get-updates";
			constexpr char getUpdatesParam[] = "/draw/get-updates/<int>";
			constexpr char sendUpdates[] = "/draw/send-updates";
			constexpr char sendUpdatesParam[] = "/draw/send-updates/<int>";
		}
	}
}