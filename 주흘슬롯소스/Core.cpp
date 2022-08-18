#include "Core.hpp"
#include "Memory.hpp"

//# DO NOT C&P IF U DUNNO HOW TO USE IT
//> he4rtbleed - https://github.com/he4rtbleed/Aeternum

namespace Aeternum
{
	uint64_t Entity::GetBoneStruct()
	{
		__try
		{
			return reinterpret_cast<uint64_t(__fastcall*)(uint64_t)>(g_Core->m_getBoneStruct)(this->Component_VELOCITY.ComponentBase);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {}
	}

	Core::Core()
	{
		auto m_getBoneStructStr = skCrypt("? 8B ? ? 8B ? E8 ? ? ? ? ? 85 ? 74 ? ? 8B ? ? 8B ? ? 8B ? E8 ? ? ? ? 81");
		m_getBoneStruct = Signature(m_getBoneStructStr).Scan().As<uint64_t>();
		m_getBoneStructStr.clear();
		auto m_refreshBonesStr = skCrypt("? 8B ? ? 8B ? E8 ? ? ? ? ? 85 ? 74 ? ? 8B ? ? 8B ? ? 8B ? E8 ? ? ? ? 81");
		m_refreshBones = Signature(m_refreshBonesStr).Scan().Add(7).Rip().As<uint64_t>();
		m_refreshBonesStr.clear();
		auto m_getSkillStructStr = skCrypt("40 53 41 56 48 83 EC ? 33 DB 4C 8B F2 66 39 5A 02");
		m_getSkillStruct = Signature(m_getSkillStructStr).Scan().As<uint64_t>();
		m_getSkillStructStr.clear();

		for (int i = 0; i < 20; i++)
		{
			if (Config::Get().RPM<uint8_t>(m_getBoneStruct - i) == 0xE8 && Config::Get().RPM<uint8_t>(m_getBoneStruct - i - 2) == 0x8B)
			{
				m_getBoneStruct = Config::Get().RPM<int32_t>(m_getBoneStruct - i + 1) + (m_getBoneStruct - i + 1) + 4;
				break;
			}
		}
	}
}