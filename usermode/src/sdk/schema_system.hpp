#pragma once

class c_schema_field_data
{
public:
	SCHEMA_ADD_STRING_OFFSET(m_name, 0x00);
	SCHEMA_ADD_OFFSET(uint16_t, m_offset, 0x10);
};

class c_schema_type_declared_class
{
public:
	SCHEMA_ADD_STRING_OFFSET(m_binary_name, 0x08);

	std::pair<uint16_t, c_schema_field_data*> get_fields() const
	{
		return
		{
			m_memory->read_t<uint16_t>(reinterpret_cast<uintptr_t>(this) + 0x1c),
			m_memory->read_t<c_schema_field_data*>(reinterpret_cast<uintptr_t>(this) + 0x28)
		};
	}
};

class c_schema_system_type_scope
{
public:
	c_utl_ts_hash<c_schema_type_declared_class*, 256, uint32_t> get_bindings_table() const
	{
		return m_memory->read_t<c_utl_ts_hash<c_schema_type_declared_class*, 256, uint32_t>>(reinterpret_cast<uintptr_t>(this) + 0x500);
	}

	std::string get_module_name() const
	{
		auto name = m_memory->read_t<std::string>(reinterpret_cast<uintptr_t>(this) + 0x08);
		if (name.empty())
			return {};

		return name;
	}
};

class c_schema_system
{
public:
	static inline c_schema_system* m_schema_system = nullptr;

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
			const auto module_name = type_scope->get_module_name();
			if (module_name.empty() || module_name.find(name) == std::string::npos)
				continue;

			return type_scope;
		}

		return {};
	}
};