#pragma once

namespace src::source2
{
    class c_utl_memory_pool
    {
    public:
        int32_t size() const
        {
            return this->m_blocks_allocated;
        }

    private:
        uint8_t m_pad0[0xc]; // 0x00
        int32_t m_blocks_allocated; // 0xc
    };

    template<class T, int Count, typename K = uintptr_t>
    class c_utl_ts_hash
    {
    public:
        int32_t size() const
        {
            return m_entry_memory.size();
        }

        int get_elements(int32_t first_element, int32_t size, uintptr_t* handles) const
        {
            int idx{ 0 };
            for (int bucket_idx{ 0 }; bucket_idx < Count; bucket_idx++)
            {
                const hash_bucket_t& hash_bucket = m_buckets[bucket_idx];

                hash_fixed_data_internal_t<T>* element = hash_bucket.m_first_uncommited;
                for (; element; element = element->get_next())
                {
                    if (--first_element >= 0)
                        continue;

                    handles[idx++] = reinterpret_cast<uintptr_t>(element);

                    if (idx >= size)
                        return idx;
                }
            }

            return idx;
        }

        T get_element(uintptr_t hash)
        {
            return static_cast<hash_fixed_data_internal_t<T>*>(reinterpret_cast<void*>(hash))->get_data();
        }

    private:
        template<typename D>
        struct hash_fixed_data_internal_t
        {
            D get_data()
            {
                return m_memory.read_t<D>(reinterpret_cast<uintptr_t>(this) + 0x10);
            }

            hash_fixed_data_internal_t<D>* get_next()
            {
                return m_memory.read_t< hash_fixed_data_internal_t<D>* >(reinterpret_cast<uintptr_t>(this) + 0x08);
            }
        };

        struct hash_bucket_t
        {
            uint8_t m_pad0[0x18]; // 0x00
            hash_fixed_data_internal_t<T>* m_first; // 0x18
            hash_fixed_data_internal_t<T>* m_first_uncommited; // 0x20
        };

        c_utl_memory_pool m_entry_memory; // 0x00
        uint8_t m_pad0[0x70]; // 0x10
        hash_bucket_t m_buckets[Count]; // 0x80
    };
}