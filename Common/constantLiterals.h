#pragma once

namespace literals
{
	const char* error = "error";

	namespace jsonKeys
	{
		namespace message
		{
			const char author[] = "author";
			const char content[] = "content";
			const char timePoint[] = "timePoint";
		}
		const char roomID[] = "gameID";
	}

	namespace routes
	{
		constexpr char baseAddress[] = "http://localhost:18080";
		constexpr char test[] = "/test";
		constexpr char gameChatParametrized[] = "/game/chat/<int>";
		constexpr char gameChat[] = "/game/chat";
	}
}