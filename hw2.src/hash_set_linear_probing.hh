/// Part of PB173 homework, created by Ondřej Budai <ondrej@budai.cz>

#ifndef HW1_HASH_LINEAR_PROBING_HH
#define HW1_HASH_LINEAR_PROBING_HH

#include <functional>
#include <vector>
#include <cassert>
#include <memory>
#include <iostream>

/// Hash set implemented using linear probing
template<typename T, typename Allocator = std::allocator<T>> class hash_set_linear_probing {
public:

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
    impl m_impl{INITIAL_HASH_TABLE_SIZE};
    static constexpr size_t INITIAL_HASH_TABLE_SIZE = 1024;
    size_t m_num_elements = 0;
    static constexpr float MAX_LOAD = 0.7f;

    enum class field_state {
        FREE, ASSIGNED, DELETED
    };

    class state_vector {
    public:
        explicit state_vector(size_t num_fields){
            m_num_fields = num_fields;
            m_data.assign(num_fields * 2, false);
        }

        void set_state(size_t field_num, field_state new_state){
            assert(field_num < m_num_fields);
            auto new_state_int = static_cast<int>(new_state);
            m_data[field_num * 2] = (new_state_int & 1) == 1;
            m_data[field_num * 2 + 1] = (new_state_int & 2) == 2;
        }

        field_state get_state(size_t field_num) const {
            assert(field_num < m_num_fields);
            auto lower_bit = static_cast<int>(m_data[field_num * 2]);
            auto higher_bit = static_cast<int>(m_data[field_num * 2 + 1]);
            auto field_state_int = lower_bit | higher_bit << 1;

            return static_cast<field_state>(field_state_int);
        }

    private:
        std::vector<bool> m_data;
        size_t m_num_fields;
    };

    /// Helping class allowing us to nicely implement hash set growth
    class impl {
    public:
        explicit impl(size_t initial_size) : m_hash_table_size{initial_size}, m_field_states{initial_size} {
            // use unique_ptr? sounds ridiculous
            // TODO: THIS IS WRONG!
            m_hash_table = {allocator.allocate(initial_size), [this](T* p){allocator.deallocate(p, m_hash_table_size);}};
//            m_hash_table = new T[initial_size];
            
//            m_hash_table.assign(initial_size, {});
        }
        
        impl(impl&) = delete;
        impl& operator=(impl&) = delete;
        impl(impl&&) noexcept = default;
        impl& operator=(impl&&) noexcept = default;
        
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
            // C++17 only :(
            // auto [n_result, free_index] = find_next_free_index(item, index);
            auto result = find_next_free_index(item, index);
            auto n_found = result.first;
            auto free_index = result.second;
            if(n_found){
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
            impl new_impl{2 * m_hash_table_size};

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
        Allocator allocator;
        size_t m_hash_table_size;
        state_vector m_field_states;
//        T* m_hash_table = nullptr;
        std::unique_ptr<T, std::function<void (T*)>> m_hash_table;
//        std::vector<T> m_hash_table;
        std::hash<T> m_hash_function;

        size_t get_first_possible_index(const T& item) const {
            auto hashed_n = m_hash_function(item);
            return hashed_n % m_hash_table_size;
        }

        /// Search for next free index for inserting a new item
        /// Return true, 0 if same item was found
        /// Return false, index of the next free index
        std::pair<bool, size_t> find_next_free_index(const T& item, size_t index) {
            while(true){
                if(m_field_states.get_state(index) != field_state::ASSIGNED){
                    return std::make_pair(false, index);
                }
                if(m_hash_table.get()[index] == item){
                    return std::make_pair(true, 0);
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
        
        struct item_search_result {
            bool found;
            size_t index;
        };
        
        item_search_result find_item_index(const T& item) const {
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
