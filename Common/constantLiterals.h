#pragma once

namespace literals
{
	const char error[] = "error";

	namespace jsonKeys
	{
		namespace message
		{
			const char author[] = "author";
			const char content[] = "content";
			const char timePoint[] = "timePoint";
		}

		namespace room 
		{
			const char ID[] = "roomID";
		}

		namespace account
		{
			const char username[] = "username";
			const char password[] = "password";
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
	}
}