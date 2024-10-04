#include "pch.hpp"

struct schema_data_t
{
	fnv1a_t m_hashed_field_name = 0;
	uint32_t m_offset = 0;
};
static std::vector<schema_data_t> m_schema_data = {};

bool schema::setup()
{
	const auto start = std::chrono::high_resolution_clock::now();

	const auto type_scope = i::m_schema_system->find_type_scope_for_module(CLIENT_DLL);
	if (!type_scope)
		return {};

	const auto table_size = type_scope->m_hash_classes().size();
	LOG_INFO("found '%d' schema classes in module '%s'", table_size, CLIENT_DLL);

	std::unique_ptr<uintptr_t[]> elements = std::make_unique_for_overwrite<uintptr_t[] >(table_size);

	const auto elements_size = type_scope->m_hash_classes().get_elements(0, table_size, elements.get());
	for (uint32_t idx = 0; idx < elements_size; idx++)
	{
		const auto element = elements[idx];
		if (!element)
			continue;

		const auto class_binding = type_scope->m_hash_classes()[element];
		if (!class_binding)
			continue;

		auto [schema_field_size, schema_field] = class_binding->get_fields();
		for (uint32_t f_idx = 0; f_idx < schema_field_size; f_idx++)
		{
			if (!schema_field)
				continue;

			auto buff = format("{}->{}", class_binding->m_binary_name(), schema_field->m_name());
			m_schema_data.emplace_back(fnv1a::hash(buff), schema_field->m_single_inheritance_offset());

			schema_field = reinterpret_cast<c_schema_class_field_data*>(reinterpret_cast<uintptr_t>(schema_field) + sizeof(c_schema_class_field_data));
		}
	}

	if (!m_schema_data.size())
		return false;

	return true;
}

uint32_t schema::get_offset(const fnv1a_t hashed_field_name)
{
	if (const auto it = std::ranges::find_if(m_schema_data, [hashed_field_name](const schema_data_t& data)
	{
		return data.m_hashed_field_name == hashed_field_name;
	});

	it != m_schema_data.end())
		return it->m_offset;

	LOG_ERROR("failed to find an offset for the field with the hash value '%d'", hashed_field_name);
	return {};
}