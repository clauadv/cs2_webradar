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

	class c_schema_field
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

	class c_schema_class_binding
	{
	public:
		string get_name() const
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

		pair<uint16_t, c_schema_field*> get_fields() const
		{
			return
			{
				m_memory.read_t<uint16_t>(reinterpret_cast<uintptr_t>(this) + 0x1c),
				m_memory.read_t<c_schema_field*>(reinterpret_cast<uintptr_t>(this) + 0x28)
			};
		}
	};

	class c_schema_system_type_scope
	{
	public:
		c_utl_ts_hash<c_schema_class_binding*, 256, uint32_t> get_bindings_table() const
		{
			return m_memory.read_t<c_utl_ts_hash<c_schema_class_binding*, 256, uint32_t>>(reinterpret_cast<uintptr_t>(this) + 0x500);
		}

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

		static uint32_t get_offset(const string& field_name)
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
			const auto size = m_memory.read_t<uint32_t>(reinterpret_cast<uintptr_t>(this) + 0x188);
			if (!size || size >= 50)
			{
				LOG_ERROR("type scope size is either empty or not good");
				return {};
			}

			const auto data = m_memory.read_t<uintptr_t>(reinterpret_cast<uintptr_t>(this) + 0x190);
			if (!data || data <= 0x10000)
			{
				LOG_ERROR("type scope data is either empty or not good");
				return {};
			}

			vector<c_schema_system_type_scope*> type_scopes{};
			type_scopes.resize(size);

			if (!m_memory.read_t(data, type_scopes.data(), size * sizeof(uintptr_t)))
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
				if (module_name.find("client.dll") == string::npos)
					continue;

				auto bindings_table = type_scope->get_bindings_table();

				unique_ptr<uintptr_t[]> elements = make_unique_for_overwrite<uintptr_t[] >(bindings_table.size());
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

						auto name = format("{}->{}", class_binding->get_name(), schema_field->get_name());
						m_schema_data.emplace_back(fnv1a::hash(name), schema_field->get_offset());

						schema_field = reinterpret_cast<c_schema_field*>(reinterpret_cast<uintptr_t>(schema_field) + 0x20);
					}
				}
			}

			if (!m_schema_data.size())
				return false;

			LOG("successfully cached '%d' schemas in %dms", m_schema_data.size(), chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count());
			return true;
		}
	};
}