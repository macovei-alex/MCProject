#pragma once

namespace literals
{
	constexpr char emptyCString[] = "";
	const std::string emptyString{ "" };
	constexpr char error[] = "error";
	constexpr char intMinStr[] = "-2147483648";

	namespace jsonKeys
	{
		namespace game
		{
			constexpr char ID[] = "gameID";
			constexpr char state[] = "state";
			constexpr char timeRemaining[] = "timer";
		}
		namespace message
		{
			constexpr char author[] = "author";
			constexpr char text[] = "text";
			constexpr char timestamp[] = "timestamp";
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

		namespace settings
		{
			constexpr char drawTime[] = "drawTime";
			constexpr char roundCount[] = "roundCount";
			constexpr char chooseWordOptionCount[] = "chooseWordOptionCount";
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

			namespace settings
			{
				constexpr char simple[] = "/game/settings";
				constexpr char param[] = "/game/settings/<int>";
			}

			namespace state
			{
				constexpr char simple[] = "/game/state";
				constexpr char param[] = "/game/state/<int>";
			}

			constexpr char create[] = "/game/create";
			constexpr char connect[] = "/game/connect";
			constexpr char connectParam[] = "/game/connect/<int>";
			constexpr char disconnect[] = "/game/disconnect";
			constexpr char disconnectParam[] = "/game/disconnect/<int>";
		}

		namespace account
		{
			constexpr char signIn[] = "/account/sign-in";
			constexpr char singUp[] = "/account/sign-up";
			constexpr char signOut[] = "/account/sign-out";
		}
	}
}