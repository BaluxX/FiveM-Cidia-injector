#pragma once
#include <cstdint>

namespace offset
{
	std::uint64_t ResourceCore = 0x6EFDD; //30/01/2022
	std::uint64_t loadfile = 0xB8110; //30/01/2022

	static uint64_t bypassAddress = 0x16480; //29/01/2022
	static uint64_t dlcmountcount = 0x69; // Maybe if it gives too many errors we should write to the offset manually :)
}
