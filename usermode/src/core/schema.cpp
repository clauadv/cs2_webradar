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

	const auto schema_system = c_schema_system::m_schema_system;
	if (!schema_system)
		return {};

	const auto type_scope = schema_system->find_type_scope_for_module("client.dll");
	if (!type_scope)
		return {};

	auto bindings_table = type_scope->get_bindings_table();

	std::unique_ptr<uintptr_t[]> elements = std::make_unique_for_overwrite<uintptr_t[] >(bindings_table.size());
	const auto elements_size = bindings_table.get_elements(0, bindings_table.size(), elements.get());

	for (auto elements_idx{ 0 }; elements_idx < elements_size; elements_idx++)
	{
		const auto element = elements[elements_idx];
		if (!element)
			continue;

		const auto class_binding = bindings_table.get_element(element);
		if (!class_binding)
			continue;

		auto [schema_field_size, schema_field] = class_binding->get_fields();
		for (auto field_idx{ 0 }; field_idx < schema_field_size; field_idx++)
		{
			if (!schema_field)
				continue;

			auto name = format("{}->{}", class_binding->m_binary_name(), schema_field->m_name());
			m_schema_data.emplace_back(fnv1a::hash(name), schema_field->m_offset());

			schema_field = reinterpret_cast<c_schema_field_data*>(reinterpret_cast<uintptr_t>(schema_field) + 0x20);
		}
	}

	if (!m_schema_data.size())
		return false;

	return true;
}

uint32_t schema::get_offset(const std::string& field_name)
{
	const auto hashed_field_name = fnv1a::hash(field_name);

	if (const auto it = std::ranges::find_if(m_schema_data, [hashed_field_name](const schema_data_t& data) { return data.m_hashed_field_name == hashed_field_name; }); it != m_schema_data.end())
		return it->m_offset;

	LOG_ERROR("failed to find offset for field '%s' with hash '0x%llx'", field_name.data(), hashed_field_name);
	return 0;
}