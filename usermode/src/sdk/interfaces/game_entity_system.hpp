#pragma once

class c_game_entity_system
{
public:
	SCHEMA_ADD_OFFSET(int32_t, m_highest_entity_idx, 0x20f0);

	template <typename T = c_base_entity*>
	T get(int32_t idx)
	{
		return m_memory->read_t<T>(this->get_entity_by_idx(idx));
	}

	template <typename T = c_base_entity*>
	T get(const c_base_handle handle)
	{
		if (!handle.is_valid())
			return nullptr;

		return m_memory->read_t<T>(this->get_entity_by_idx(handle.get_entry_idx()));
	}

private:
	void* get_entity_by_idx(const int32_t idx)
	{
		if (static_cast<uint32_t>(idx) >= 0x7ffe)
			return nullptr;

		if (static_cast<uint32_t>(idx >> 9) >= 0x3f)
			return nullptr;

		const auto entry_list = m_memory->read_t<uintptr_t>(this + 8i64 * (idx >> 9) + 16);
		if (!entry_list)
			return nullptr;

		const auto player_controller = (uint32_t*)(120i64 * (idx & 0x1ff) + entry_list);
		if (!player_controller)
			return nullptr;

		return player_controller;
	}
};