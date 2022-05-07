#pragma once
class CoolUUID
{
public:
	CoolUUID();
	CoolUUID(uint64_t uiiD);

	uint64_t operator*() const
	{
		return m_uiUUID;
	}

private:
	uint64_t m_uiUUID;
};

namespace std
{
	template<>
	struct hash<CoolUUID>
	{
		std::size_t operator()(const CoolUUID& kuUID)
		{
			return hash<uint64_t>()(kuUID.operator*());
		}
	};
}