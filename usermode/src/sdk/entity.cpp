#include "pch.hpp"

const c_base_handle c_entity_instance::get_ref_e_handle()
{
	const auto entity = m_pEntity();
	if (!entity)
		return c_base_handle();

	return c_base_handle(entity->get_entry_idx(), entity->get_serial_number() - (entity->m_flags() & 1));
}

const std::string c_entity_instance::get_schema_class_name()
{
	const auto entity = m_pEntity();
	if (!entity)
		return {};

	const auto class_info = entity->m_pClassInfo();
	if (!class_info)
		return {};

	const auto unk_1 = m_memory->read_t<uintptr_t>(class_info + 0x28);
	if (!unk_1)
		return {};

	const auto unk_2 = m_memory->read_t<uintptr_t>(unk_1 + 0x08);
	if (!unk_2)
		return {};

	auto name = m_memory->read_t<std::string>(unk_2);
	if (name.empty())
		return {};

	return name;
}

const std::string c_cs_player_pawn::get_model_name()
{
	const auto model_name = m_memory->read_t<uintptr_t>(m_pGameSceneNode() + SCHEMA_GET_OFFSET("CSkeletonInstance->m_modelState") + SCHEMA_GET_OFFSET("CModelState->m_ModelName"));
	if (!model_name)
		return {};

	const auto model_path = m_memory->read_t<std::string>(model_name);
	if (model_path.empty())
		return {};

	return model_path.substr(model_path.rfind("/") + 1, model_path.rfind(".") - model_path.rfind("/") - 1);
}

const e_colors c_cs_player_controller::get_color()
{
	const auto color = m_iCompTeammateColor();
	if (color == static_cast<e_colors>(-1))
		return e_colors::white;

	return color;
}

const fvector3& c_cs_player_controller::get_vec_origin()
{
	const auto entity_list = c_game_entity_system::m_entity_list;
	if (!entity_list)
		return {};

	const auto pawn_handle = m_hPawn();
	if (!pawn_handle.is_valid())
		return {};

	const auto pawn = entity_list->get<c_cs_player_pawn*>(pawn_handle);
	if (!pawn)
		return {};

	return pawn->get_scene_origin();
}

const fvector3& c_base_entity::get_scene_origin()
{
	if (m_pGameSceneNode())
		return m_pGameSceneNode()->m_vecAbsOrigin();

	return {};
}

c_base_player_weapon* c_base_player_weapon::get(const int32_t idx)
{
	const auto entity_list = c_game_entity_system::m_entity_list;
	if (!entity_list)
		return nullptr;

	const auto handle = m_memory->read_t<int32_t>(this + idx * 0x4);
	if (handle == -1)
		return nullptr;

	return entity_list->get<c_base_player_weapon*>(handle & ENT_ENTRY_MASK);
}