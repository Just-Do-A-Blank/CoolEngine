#include "UUID.h"

#include <random>

static std::random_device s_randomDevice;
static std::mt19937_64 s_randomEngine = std::mt19937_64(s_randomDevice());
static std::uniform_int_distribution<UINT64> s_uniformDistribution;

CoolUUID::CoolUUID()
{
	m_uiUUID = s_uniformDistribution(s_randomEngine);
}

CoolUUID::CoolUUID(uint64_t uiiD)
{
	m_uiUUID = uiiD;
}