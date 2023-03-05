#pragma once

#include <inttypes.h>
#include <typeindex>

	class UniqueID
	{
	public:
		UniqueID();
		UniqueID(uint64_t uuid);
		UniqueID(const UniqueID& other);
		bool isValid() { return m_UUID > 0; }
		operator uint64_t () { return m_UUID; }
		operator const uint64_t() const { return m_UUID; }
	private:
		uint64_t m_UUID;
	};

namespace std {

	template <>
	struct hash<UniqueID>
	{
		std::size_t operator()(const UniqueID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
}