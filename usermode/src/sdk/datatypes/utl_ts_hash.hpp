#pragma once

class c_utl_memory_pool
{
public:
    int32_t size() const
    {
        return this->m_blocks_allocated;
    }

private:
    uint8_t m_pad0[0xc]; // 0x00(0x0c)
    int32_t m_blocks_allocated; // 0xc(0x04)
};
static_assert(sizeof(c_utl_memory_pool) == 0x10, "wrong size on c_utl_memory_pool");

template<class T, int Count, typename K = uintptr_t>
class c_utl_ts_hash
{
public:
    uint32_t size() const
    {
        return m_entry_memory.size();
    }

    uint32_t get_elements(int32_t first_element, uint32_t size, uintptr_t* handles) const
    {
        uint32_t element_idx = 0;

        for (uint32_t idx = 0; idx < Count; idx++)
        {
            const hash_bucket_t& hash_bucket = m_buckets[idx];

            hash_fixed_data_internal_t<T>* element = hash_bucket.m_first_uncommited;
            for (; element; element = element->m_next())
            {
                if (--first_element >= 0)
                    continue;

                handles[element_idx++] = reinterpret_cast<uintptr_t>(element);

                if (element_idx >= size)
                    return element_idx;
            }
        }

        return element_idx;
    }

    T operator[](uintptr_t hash)
    {
        return reinterpret_cast<hash_fixed_data_internal_t<T>*>(hash)->m_data();
    }

private:
    template<typename D>
    struct hash_fixed_data_internal_t
    {
        D m_data()
        {
            return m_memory->read_t<D>(reinterpret_cast<uintptr_t>(this) + 0x10);
        }

        hash_fixed_data_internal_t<D>* m_next()
        {
            return m_memory->read_t< hash_fixed_data_internal_t<D>* >(reinterpret_cast<uintptr_t>(this) + 0x08);
        }
    };

    struct hash_bucket_t
    {
        uint8_t m_pad0[0x18]; // 0x00
        hash_fixed_data_internal_t<T>* m_first; // 0x18
        hash_fixed_data_internal_t<T>* m_first_uncommited; // 0x20
    };

    c_utl_memory_pool m_entry_memory; // 0x00(0x10)
    uint8_t m_pad0[0x70]; // 0x10(0x70)
    hash_bucket_t m_buckets[Count]; // 0x80(0x100)
};