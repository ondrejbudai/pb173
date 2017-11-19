#ifndef HW4_INT16_SET_TRIE_HH
#define HW4_INT16_SET_TRIE_HH

#include <cstdint>
#include <memory>
#include <optional>
#include <algorithm>

namespace detail {
    template<unsigned level>
    class node {
    public:
        node() = default;
        node(const node& rhs){
            using std::cbegin, std::cend, std::begin;

            std::transform(
                cbegin(rhs.children),
                cend(rhs.children),
                begin(children),
                [](const auto& child) {
                    if(child.get()) {
                        return std::make_unique<node<level - 1>>(*child);
                    }

                    return std::unique_ptr<node<level-1>>{};
                }
            );
        }

        node& operator=(node&& rhs) noexcept = default;

        bool contains(uint16_t value) const {
            auto nibble = value & 0xf;

            auto& child = children[nibble];
            if(!child) {
                return false;
            }

            return child->contains(value >> 4);
        }

        void insert(uint16_t value) {
            auto nibble = value & 0x0f;

            auto& child = children[nibble];

            if(!child){
                child = std::make_unique<node<level -1>>();
            }

            child->insert(value >> 4);
        }

        void operator|=(const node<level>& rhs){
            for(auto i = 0; i < 16; ++i){
                auto& child = children[i];
                auto& rhs_child = rhs.children[i];

                if(!rhs_child){
                    continue;
                }

                if(!child){
                    child = std::make_unique<node<level - 1>>();
                }

                *child |= *rhs_child;
            }
        }


    private:
        std::array<std::unique_ptr<node<level - 1>>, 16> children{};
    };

    template<>
    class node<0> {
    public:
        bool contains(uint16_t value) const {
            return static_cast<bool>(data & (1 << value));
        }

        void insert(uint16_t value) {
            data |= 1 << value;
        }

        void operator|=(const node<0>& rhs){
            data |= rhs.data;
        }

    private:
        uint16_t data = 0;
    };
}

class int16_set_trie {
public:

    void insert(uint16_t value){
        parent.insert(value);
    }

    bool contains(uint16_t value) const {
        return parent.contains(value);
    }
    int16_set_trie operator|(const int16_set_trie& rhs) const {
        int16_set_trie result = *this;

        result.parent |= rhs.parent;

        return result;
    }
private:
    detail::node<3> parent;
};

#endif //HW4_INT16_SET_TRIE_HH
