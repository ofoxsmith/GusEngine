#pragma once
#include <compare>
#include <stdint.h>
// An integer id used to uniquely identify objects.
class Id {
	friend class IdGen;
	private:
	unsigned int _id;
	unsigned int _genId;
	Id(unsigned int id, unsigned int genId): _id(id), _genId(genId) {}

	public:
	Id() : _id(-1), _genId(-1) {}
	std::partial_ordering operator<=>(const Id& other) const{
		if (_genId != other._genId) return std::partial_ordering::unordered;
		return _id <=> other._id;
	}
};

class IdGen {
	private:
	unsigned int _currentId;
	unsigned int _genId;
	static unsigned int _nextGenId;

	public:
	IdGen() {
		_genId = _nextGenId++;
		_currentId = 1;
	}

	Id Next() {
		return Id(_currentId++, _genId);
	}
};