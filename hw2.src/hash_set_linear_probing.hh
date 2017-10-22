/// Part of PB173 homework, created by Ondřej Budai <ondrej@budai.cz>

#ifndef HW1_HASH_LINEAR_PROBING_HH
#define HW1_HASH_LINEAR_PROBING_HH

#include <functional>
#include <vector>
#include <memory>

/// Hash set implemented using linear probing
template<typename T, typename Allocator = std::allocator<T>> class hash_set_linear_probing {
public:

    hash_set_linear_probing() : hash_set_linear_probing(Allocator()){}

    explicit hash_set_linear_probing(const Allocator& allocator) : m_allocator{allocator}, m_impl{INITIAL_HASH_TABLE_SIZE, m_allocator} {}

    /// Insert item into set
    void insert(const T& item){
        bool actually_inserted = m_impl.insert(item);
        if(!actually_inserted){
            return;
        }
        ++m_num_elements;
        // If number elements grows sufficiently, grow the bucket count
        // Please note without resizing the implementation will break!
        if(m_num_elements > MAX_LOAD * m_impl.size()){
            m_impl = m_impl.create_bigger_self();
        }
    }

    /// Searches for item in set
    bool find(const T& item) const{
        return m_impl.find(item);
    }

    void erase(const T& item){
        m_impl.erase(item);
    }

private:
    class impl;
    Allocator m_allocator;
    impl m_impl;
    static constexpr size_t INITIAL_HASH_TABLE_SIZE = 1024;
    size_t m_num_elements = 0;
    static constexpr float MAX_LOAD = 0.7f;

    enum class field_state {
        FREE, ASSIGNED, DELETED
    };

    class state_vector {
    public:
        explicit state_vector(size_t num_fields){
            m_data.assign(num_fields * 2, false);
        }

        void set_state(size_t field_num, field_state new_state){
            auto new_state_int = static_cast<int>(new_state);
            m_data[field_num * 2] = (new_state_int & 1) == 1;
            m_data[field_num * 2 + 1] = (new_state_int & 2) == 2;
        }

        field_state get_state(size_t field_num) const {
            auto lower_bit = static_cast<int>(m_data[field_num * 2]);
            auto higher_bit = static_cast<int>(m_data[field_num * 2 + 1]);
            auto field_state_int = lower_bit | higher_bit << 1;

            return static_cast<field_state>(field_state_int);
        }

    private:
        std::vector<bool> m_data;
    };

    /// Helping class allowing us to nicely implement hash set growth
    class impl {
    public:
        impl(size_t initial_size, Allocator& allocator) : m_hash_table_size{initial_size}, m_field_states{initial_size}, m_allocator{allocator} {
            m_hash_table = {allocator.allocate(initial_size),
                            [&allocator, initial_size](T* p) mutable {
                                allocator.deallocate(p, initial_size);
                            }
            };
        }

        impl(impl&) = delete;
        impl& operator=(impl&) = delete;
        impl(impl&&) noexcept = default;
        impl& operator=(impl&& other) noexcept {
            this->~impl();
            new (this) impl(std::move(other));
            return *this;
        }

        ~impl(){
            if(!m_hash_table){
                return;
            }
            for(size_t i = 0; i < m_hash_table_size; ++i){
                if(m_field_states.get_state(i) != field_state::ASSIGNED){
                    continue;
                }
                m_hash_table.get()[i].~T();
            }
        }

        bool insert(const T& item){
            const auto index = get_first_possible_index(item);

            auto [found, free_index] = find_next_free_index(item, index);
            if(found){
                return false;
            }
            insert_to_index(item, free_index);
            return true;
        }

        bool find(const T& item) const {
            return find_item_index(item).found;
        }

        void erase(const T& item){
            auto [found, index] = find_item_index(item);
            if (!found){
                return;
            }

            m_field_states.set_state(index, field_state::DELETED);
        }

        /// Creates impl with same items but bigger table
        impl create_bigger_self(){
            impl new_impl{2 * m_hash_table_size, m_allocator};

            for(size_t i = 0; i < m_hash_table_size; ++i){
                if(m_field_states.get_state(i) != field_state::ASSIGNED){
                    continue;
                }
                new_impl.insert(m_hash_table.get()[i]);
            }

            return new_impl;
        }

        size_t size(){return m_hash_table_size;}
    private:
        size_t m_hash_table_size;
        state_vector m_field_states;
        std::unique_ptr<T, std::function<void (T*)>> m_hash_table;
        std::hash<T> m_hash_function;
        Allocator& m_allocator;

        size_t get_first_possible_index(const T& item) const {
            auto hashed_n = m_hash_function(item);
            return hashed_n % m_hash_table_size;
        }

        struct item_index_search_result {
            bool found;
            size_t index;
        };

        /// Search for next free index for inserting a new item
        /// Return true, 0 if same item was found
        /// Return false, index of the next free index
        item_index_search_result find_next_free_index(const T& item, size_t index) {
            while(true){
                if(m_field_states.get_state(index) != field_state::ASSIGNED){
                    return {false, index};
                }
                if(m_hash_table.get()[index] == item){
                    return {true, 0};
                }
                ++index;
                if(index >= m_hash_table_size){
                    index = 0;
                }
            }
        }

        void insert_to_index(const T& item, size_t index){
            m_field_states.set_state(index, field_state::ASSIGNED);
            auto field_ptr = &m_hash_table.get()[index];
            new (field_ptr) T(item);
        }

        item_index_search_result find_item_index(const T& item) const {
            auto index = get_first_possible_index(item);
            while(true){
                if(m_field_states.get_state(index) == field_state::FREE){
                    return {false, 0};
                }
                if(m_hash_table.get()[index] == item){
                    return {true, index};
                }
                ++index;
                if(index >= m_hash_table_size){
                    index = 0;
                }
            }
        }
    };
};

#endif //HW1_HASH_LINEAR_PROBING_HH
