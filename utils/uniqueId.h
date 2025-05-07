#pragma once
#include <compare>
#include <stdint.h>
// An integer id used to uniquely identify objects.
class Id {
	private:
	uint32_t _id;

	public:

	std::strong_ordering operator<=>(const Id& other) const{
		return _id <=> other._id;
	}
};