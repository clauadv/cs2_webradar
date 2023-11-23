#pragma once

#define GET_SCHEMA(name) \
	source2::c_schema_system::get_offset(name)

namespace src::source2
{
	struct schema_data_t
	{
		uint64_t m_hashed_field_name;
		uint32_t m_offset;
	};
	inline vector<schema_data_t> m_schema_data;

	class c_schema_field_data
	{
	public:
		string get_name() const
		{
			const auto ptr = m_memory.read_t<uintptr_t>(reinterpret_cast<uintptr_t>(this));
			if (!ptr)
				return string{};

			auto name = m_memory.read_t<string>(ptr);
			if (name.empty())
				return string{};

			ranges::transform(name, name.begin(), ::tolower);

			return name;
		}

		uint16_t get_offset() const
		{
			return m_memory.read_t<uint16_t>(reinterpret_cast<uintptr_t>(this) + 0x10);
		}
	};

	class c_schema_class_info
	{
	public:
		uint16_t get_fields_size() const
		{
			return m_memory.read_t<uint16_t>(reinterpret_cast<uintptr_t>(this) + 0x1c);
		}
	};

	class c_schema_type_declared_class
	{
	public:
		string get_binary_name() const
		{
			const auto ptr = m_memory.read_t<uintptr_t>(reinterpret_cast<uintptr_t>(this) + 0x08);
			if (!ptr)
				return string{};

			auto name = m_memory.read_t<string>(ptr);
			if (name.empty())
				return string{};

			ranges::transform(name, name.begin(), ::tolower);

			return name;
		}

		string get_module_name() const
		{
			const auto ptr = m_memory.read_t<uintptr_t>(reinterpret_cast<uintptr_t>(this) + 0x10);
			if (!ptr)
				return string{};

			auto name = m_memory.read_t<string>(ptr);
			if (name.empty())
				return string{};

			ranges::transform(name, name.begin(), ::tolower);

			return name;
		}
	};

	class c_schema_system_type_scope
	{
	public:
		string get_module_name() const
		{
			auto name = m_memory.read_t<string>(reinterpret_cast<uintptr_t>(this) + 0x08);
			if (name.empty())
				return string{};

			ranges::transform(name, name.begin(), ::tolower);

			return name;
		}
	};

	class c_schema_system
	{
	public:
		static c_schema_system* get()
		{
			const auto schema_system = m_memory.find_pattern(SCHEMASYSTEM_DLL, GET_SCHEMA_SYSTEM);
			if (!schema_system.has_value())
				return nullptr;

			return schema_system->rip().as<c_schema_system*>();
		}

		static uint32_t get_offset(const std::string& field_name)
		{
			const auto hashed_field_name = fnv1a::hash(field_name);
			auto last_offset{ 0 };

			for (const auto& entry : m_schema_data)
			{
				if (entry.m_hashed_field_name == hashed_field_name)
					last_offset = entry.m_offset;
			}

			if (!last_offset)
				LOG_ERROR("failed to find offset for field '%s' with hash '%llu'", field_name.data(), hashed_field_name);

			return last_offset;
		}

		vector<c_schema_system_type_scope*> get_type_scopes() const
		{
			const auto count = m_memory.read_t<uint32_t>(reinterpret_cast<uintptr_t>(this) + 0x190);
			if (!count)
				return {};

			const auto data = m_memory.read_t<uintptr_t>(reinterpret_cast<uintptr_t>(this) + 0x198);
			if (!data)
				return {};

			vector<c_schema_system_type_scope*> type_scopes{};
			type_scopes.resize(count);

			if (!m_memory.read_t(data, type_scopes.data(), count * sizeof(uintptr_t)))
				return {};

			return type_scopes;
		}

		static bool setup()
		{
			const auto start = chrono::high_resolution_clock::now();

			const auto schema_system = c_schema_system::get();
			if (!schema_system)
				return false;

			for (const auto& type_scope : schema_system->get_type_scopes())
			{
				const auto module_name = type_scope->get_module_name();
				if (module_name.empty())
					continue;

				const auto classes = m_memory.read_t<c_utl_ts_hash<c_schema_type_declared_class*>>(reinterpret_cast<uintptr_t>(type_scope) + 0x588);
				for (const auto& _class : classes.elements())
				{
					const auto& class_name = _class->get_binary_name();

					const auto class_info = reinterpret_cast<c_schema_class_info*>(_class);
					for (auto idx{ 0 }; idx < class_info->get_fields_size(); ++idx)
					{
						const auto field = reinterpret_cast<c_schema_field_data*>(m_memory.read_t<uintptr_t>(reinterpret_cast<uintptr_t>(class_info) + 0x28) + (idx * 0x20));
						if (!field)
							continue;

						auto name = format("{}->{}", class_name, field->get_name());
						m_schema_data.emplace_back(fnv1a::hash(name), field->get_offset());
					}
				}
			}

			LOG("successfully cached '%d' schemas in %dms", m_schema_data.size(), chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count());
			return true;
		}
	};
}