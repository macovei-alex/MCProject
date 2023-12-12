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
			constexpr char points[] = "points";
		}
	}

	namespace routes
	{
		constexpr char baseAddress[] = "http://localhost:18080";
		constexpr char test[] = "/test";

		namespace game 
		{
			namespace chat
			{
				constexpr char simple[] = "/game/chat";
				constexpr char param[] = "/game/chat/<int>";
			}

			namespace draw
			{
				constexpr char updates[] = "/game/draw/updates";
				constexpr char updatesParam[] = "/game/draw/updates/<int>";
			}
		}

		namespace room
		{
			constexpr char create[] = "/room/create";
			constexpr char connect[] = "/room/connect";
			constexpr char connectParam[] = "/room/connect/<int>";
			constexpr char disconnect[] = "/room/disconnect";
			constexpr char disconnectParam[] = "/room/disconnect/<int>";
		}

		namespace account
		{
			constexpr char signIn[] = "/account/sign-in";
			constexpr char singUp[] = "/account/sign-up";
			constexpr char signOut[] = "/account/sign-out";
		}
	}
}