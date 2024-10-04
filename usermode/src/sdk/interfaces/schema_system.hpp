#pragma once

class c_schema_class_field_data
{
public:
	SCHEMA_ADD_STRING_OFFSET(m_name, 0x00);
	SCHEMA_ADD_OFFSET(uint16_t, m_single_inheritance_offset, 0x10);
	uint8_t m_pad0[0x20];
};
static_assert(sizeof(c_schema_class_field_data) == 0x20, "wrong size on c_schema_class_field_data");

class c_schema_type_declared_class
{
public:
	SCHEMA_ADD_STRING_OFFSET(m_binary_name, 0x08);

	std::pair<uint16_t, c_schema_class_field_data*> get_fields() const
	{
		return
		{
			m_memory->read_t<uint16_t>(reinterpret_cast<uintptr_t>(this) + 0x1c),
			m_memory->read_t<c_schema_class_field_data*>(reinterpret_cast<uintptr_t>(this) + 0x28)
		};
	}
};

class c_schema_system_type_scope
{
public:
	std::string m_module_name() const
	{
		auto name = m_memory->read_t<std::string>(reinterpret_cast<uintptr_t>(this) + 0x08);
		if (name.empty())
			return {};

		return name;
	}

	c_utl_ts_hash<c_schema_type_declared_class*, 256, uint32_t> m_hash_classes() const
	{
		return m_memory->read_t<c_utl_ts_hash<c_schema_type_declared_class*, 256, uint32_t>>(reinterpret_cast<uintptr_t>(this) + 0x500);
	}
};

class c_schema_system
{
public:
	class c_schema_system_type_scope* find_type_scope_for_module(const std::string_view& name) const
	{
		const auto size = m_memory->read_t<uint32_t>(reinterpret_cast<uintptr_t>(this) + 0x188);
		if (!size || size >= 50)
		{
			LOG_ERROR("type scope size is either empty or not good");
			return {};
		}

		const auto data = m_memory->read_t<uintptr_t>(reinterpret_cast<uintptr_t>(this) + 0x190);
		if (!data || data <= 0x10000)
		{
			LOG_ERROR("type scope data is either empty or not good");
			return {};
		}

		std::vector<c_schema_system_type_scope*> type_scopes{};
		type_scopes.resize(size);

		if (!m_memory->read_t(data, type_scopes.data(), size * sizeof(uintptr_t)))
			return {};

		for (const auto& type_scope : type_scopes)
		{
			const auto module_name = type_scope->m_module_name();
			if (module_name.empty() || module_name.find(name) == std::string::npos)
				continue;

			return type_scope;
		}

		return {};
	}
};