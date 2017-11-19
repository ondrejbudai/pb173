#ifndef HW4_INT16_SET_BITVECTOR_HH
#define HW4_INT16_SET_BITVECTOR_HH

#include <array>
#include <climits>

namespace detail {
    template<typename BaseType, typename ExpType>
    constexpr auto pow(BaseType base, ExpType exp) noexcept {
        BaseType result = 1;
        for(; exp > 0; --exp){
            result *= base;
        }

        return result;
    }
}

class int16_set_bitvector {
    using ValueType = uint16_t;
    constexpr static auto possible_value_count = detail::pow(std::size_t{2}, sizeof(ValueType) * CHAR_BIT);
    using UnderlyingType = uint64_t;
    constexpr static auto bits_per_bucket = sizeof(UnderlyingType) * CHAR_BIT;
    constexpr static auto bucket_count = possible_value_count / bits_per_bucket;
public:
    void insert(ValueType value){
        m_data[value / bits_per_bucket] |= 1 << (value % bits_per_bucket);
    }
    void erase(ValueType value){
        m_data[value / bits_per_bucket] &= ~(1 << (value % bits_per_bucket));
    }
    bool contains(ValueType value) const {
        return static_cast<bool>(m_data[value / bits_per_bucket] & (1 << (value % bits_per_bucket)));
    }

    int16_set_bitvector operator&(const int16_set_bitvector& rhs) const {
        int16_set_bitvector result;

        for(std::size_t index = 0; index < bucket_count; ++index){
            result.m_data[index] = m_data[index] & rhs.m_data[index];
        }

        return result;
    }
    int16_set_bitvector operator|(const int16_set_bitvector& rhs) const {
        int16_set_bitvector result;

        for(std::size_t index = 0; index < bucket_count; ++index){
            result.m_data[index] = m_data[index] | rhs.m_data[index];
        }

        return result;
    }

private:
    std::array<uint64_t, bucket_count> m_data{};
};

#endif //HW4_INT16_SET_BITVECTOR_HH
