/// Part of PB173 homework, created by Ondřej Budai <ondrej@budai.cz>

#ifndef HW1_HASH_LINKED_LIST_HH
#define HW1_HASH_LINKED_LIST_HH

#include <functional>
#include <vector>
#include <algorithm>

/// Hash set implemented using linked list (although internally std::vector is used)
template<typename T> class hash_set_linked_list {
public:

    /// Inserts item into set
    void insert(const T& item){
        auto actually_inserted = m_impl.insert(item);
        if(!actually_inserted) {
            return;
        }

        ++m_num_elements;

        // If number elements grows sufficiently, grow the bucket count
        if(m_num_elements > m_impl.bucket_count() / GROW_FACTOR){
            // create impl with greater bucket count and replace current impl with it
            impl new_impl{m_impl.create_bigger_self()};
            m_impl = std::move(new_impl);
        }
    }

    /// Searches for item in set
    bool find(const T& item) const {
        return m_impl.find(item);
    }

private:
    class impl;
    impl m_impl{INITIAL_BUCKET_NUMBER};
    constexpr static size_t INITIAL_BUCKET_NUMBER = 10;
    constexpr static float GROW_FACTOR = 2;
    size_t m_num_elements = 0;

    /// Helping class allowing us to nicely implement bucket count growth
    class impl {
    public:
        explicit impl(size_t bucket_number) : m_bucket_count{bucket_number} {
            m_buckets.assign(m_bucket_count, {});
        }
        bool insert(const T& item){
            auto& bucket = get_bucket(item);
            if(find_in_bucket(bucket, item)){
                return false;
            }
            bucket.emplace_back(item);
            return true;
        }

        bool find(const T& item) const {
            const auto& bucket = get_bucket(item);
            return find_in_bucket(bucket, item);
        }

        size_t bucket_count(){
            return m_bucket_count;
        }

        /// Creates impl with same items but bigger bucket count
        impl create_bigger_self(){
            impl new_impl{static_cast<size_t>(GROW_FACTOR * m_bucket_count)};
            for(const auto& bucket: m_buckets){
                for(const auto& item: bucket){
                    new_impl.insert(item);
                }
            }
            return new_impl;
        }

    private:
        std::vector<std::vector<T>> m_buckets;
        size_t m_bucket_count;
        static std::hash<T> hash_function;

        std::vector<T>& get_bucket(const T& item) {
            auto hashed_n = hash_function(item);
            auto bucket = hashed_n % m_bucket_count;
            return m_buckets[bucket];
        }

        const std::vector<T>& get_bucket(const T& item) const {
            auto hashed_n = hash_function(item);
            auto bucket = hashed_n % m_bucket_count;
            return m_buckets[bucket];
        }

        bool find_in_bucket(const std::vector<T>& bucket, const T& item) const {
            return std::find(std::begin(bucket), std::end(bucket), item) != std::end(bucket);
        }
    };
};

#endif //HW1_HASH_LINKED_LIST_HH
