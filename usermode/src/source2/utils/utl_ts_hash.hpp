#pragma once

namespace src::source2
{
    template <class T, typename K>
    class c_hash_bucket_data_internal
    {
    public:
        c_hash_bucket_data_internal<T, K>* next() const
        {
            return m_memory.read_t<c_hash_bucket_data_internal<T, K>*>(reinterpret_cast<uintptr_t>(this) + 0x08);
        }

    public:
        T m_data; // 0x00
        uint8_t m_pad_0[0x08]; // 0x08
        K m_ui_key; // 0x10
    };

    template <class T, typename K>
    class c_hash_fixed_data_internal
    {
    public:
        c_hash_fixed_data_internal<T, K>* next() const
        {
            return m_memory.read_t<c_hash_fixed_data_internal<T, K>*>(reinterpret_cast<uintptr_t>(this) + 0x08);
        }

    public:
        K m_ui_key; // 0x00
        uint8_t m_pad_0[0x08]; // 0x08
        T m_data; // 0x10
    };

    template <class T, typename K>
    struct hash_allocated_data_t
    {
        array<c_hash_fixed_data_internal<T, K>, 128> list() const
        {
            return m_memory.read_t<array<c_hash_fixed_data_internal<T, K>, 128>>(reinterpret_cast<uintptr_t>(this) + 0x18);
        }
    };

    template <class T, typename K>
    class c_hash_unallocated_data
    {
    public:
        c_hash_unallocated_data<T, K>* next() const
        {
            return m_memory.read_t<c_hash_unallocated_data<T, K>*>(reinterpret_cast<uintptr_t>(this));
        }

        K ui_key() const
        {
            return m_memory.read_t<K>(reinterpret_cast<uintptr_t>(this) + 0x10);
        }

        array<c_hash_bucket_data_internal<T, K>, 256> block_list() const
        {
            return m_memory.read_t<array<c_hash_bucket_data_internal<T, K>, 256>>(reinterpret_cast<uintptr_t>(this) + 0x20);
        }
    };

    template <class T, typename K>
    struct hash_bucket_t
    {
        uint8_t pad_0[0x10]; // 0x00
        hash_allocated_data_t<T, K>* m_allocated_data; // 0x10
        c_hash_unallocated_data<T, K>* m_unallocated_data; // 0x18
    };

    class c_utl_memory_pool
    {
    public:
        int32_t block_size() const
        {
            return this->m_blocks_per_blob;
        }

        int32_t size() const
        {
            return this->m_block_allocated_size;
        }

    private:
        int32_t m_block_size; // 0x00
        int32_t m_blocks_per_blob; // 0x04
        int32_t m_grow_mode; // 0x08
        int32_t m_blocks_allocated; // 0xc
        int32_t m_block_allocated_size; // 0x10
        int32_t m_peak_alloc; // 0x14
    };

    template <class T, typename K = uintptr_t>
    class c_utl_ts_hash
    {
    public:
        int32_t block_size() const
        {
            return this->m_entry_memory.block_size();
        }

        int32_t size() const
        {
            return this->m_entry_memory.size();
        }

        vector<T> elements() const
        {
            vector<T> list;
            const auto& unallocated_data = this->m_buckets.m_unallocated_data;
            auto idx{ 0 };

            for (auto element = unallocated_data; element != nullptr; element = element->next())
            {
                const auto block_list = element->block_list();

                for (auto i{ 0 }; i < this->block_size() && i != this->size(); ++i)
                {
                    list.emplace_back(block_list[i].m_data);

                    ++idx;

                    if (idx >= this->size())
                        break;
                }
            }

            return list;
        }

    private:
        c_utl_memory_pool m_entry_memory; // 0x00
        hash_bucket_t<T, K> m_buckets; // 0x18
    };
}