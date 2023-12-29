#include "servicesUtils.h"

chr::time_point<chr::system_clock, chr::seconds> utils::DateTimeFromInteger(uint64_t millis)
{
	return chr::time_point<chr::system_clock, chr::seconds>
		(chr::duration_cast<chr::seconds>
			(chr::milliseconds{ millis }));
}

uint64_t utils::NowAsInteger()
{
	return chr::duration_cast<chr::milliseconds>
		(chr::system_clock::now().time_since_epoch()).count();
}
