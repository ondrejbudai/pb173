/// Part of PB173 homework, created by Ondřej Budai <ondrej@budai.cz>

#ifndef HW1_HASH_LINEAR_PROBING_HH
#define HW1_HASH_LINEAR_PROBING_HH

#include <functional>
#include <vector>

/// Hash set implemented using linear probing
template<typename T> class hash_set_linear_probing {
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
            impl new_impl{m_impl.create_bigger_self()};
            m_impl = std::move(new_impl);
        }
    }

    /// Searches for item in set
    bool find(const T& item) const{
        return m_impl.find(item);
    }

private:
    class impl;
    impl m_impl{INITIAL_HASH_TABLE_SIZE};
    static constexpr size_t INITIAL_HASH_TABLE_SIZE = 1024;
    size_t m_num_elements = 0;
    static constexpr float MAX_LOAD = 0.7f;

    /// Helping class allowing us to nicely implement hash set growth
    class impl {
    public:
        explicit impl(size_t initial_size){
            hash_table_size = initial_size;
            occupied_table.assign(hash_table_size, false);
            hash_table.assign(hash_table_size, {});
        }
        bool insert(const T& item){
            const auto index = get_index(item);
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
            auto index = get_index(item);
            while(true){
                if(!occupied_table[index]){
                    return false;
                }
                if(hash_table[index] == item){
                    return true;
                }
                ++index;
                if(index >= hash_table_size){
                    index = 0;
                }
            }
        }

        /// Creates impl with same items but bigger table
        impl create_bigger_self(){
            impl new_impl{2 * hash_table_size};

            for(size_t i = 0; i < hash_table_size; ++i){
                if(!occupied_table[i]){
                    continue;
                }
                new_impl.insert(hash_table[i]);
            }

            return new_impl;
        }

        size_t size(){return hash_table_size;}
    private:
        size_t hash_table_size;
        std::vector<bool> occupied_table;
        std::vector<T> hash_table;
        std::hash<T> hash_function;

        size_t get_index(const T& item) const {
            auto hashed_n = hash_function(item);
            return hashed_n % hash_table_size;
        }

        /// Search for next free index for inserting a new item
        /// Return true, 0 if same item was found
        /// Return false, index of the next free index
        std::pair<bool, size_t> find_next_free_index(const T& item, size_t index) {
            while(true){
                if(!occupied_table[index]){
                    return std::make_pair(false, index);
                }
                if(hash_table[index] == item){
                    return std::make_pair(true, 0);
                }
                ++index;
                if(index >= hash_table_size){
                    index = 0;
                }
            }
        }

        void insert_to_index(const T& item, size_t index){
            occupied_table[index] = true;
            hash_table[index] = item;
        }
    };
};

#endif //HW1_HASH_LINEAR_PROBING_HH
