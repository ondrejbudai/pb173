#include <iostream>
#include <vector>
#include <set>
#include <ctime>
#include <algorithm>
#include <cassert>

template<typename T> class buffer_set {
public:
    explicit buffer_set(size_t capacity) {
        m_capacity = capacity;
    }
    void insert(T n) {
        m_data.insert(n);
        if(m_data.size() > m_capacity){
            m_data.erase(m_data.begin());
        }
    }
    T top() {
        return *(--m_data.end());
    }
    void remove(){
        m_data.erase(--m_data.end());
    }

private:
    std::set<T> m_data;
    size_t m_capacity;
};

template<typename T> class buffer_vector {
public:
    explicit buffer_vector(size_t capacity) {
        m_capacity = capacity;
        m_data.reserve(capacity);
    }
    void insert(T n) {
        if(m_data.empty()){
            m_data.emplace_back(n);
            return;
        }
        auto pos = std::find_if(std::begin(m_data), std::end(m_data),
                                [&](auto item){
                                    return item < n;
                                });
        if(n == *(pos - 1)){
            return;
        }
        m_data.insert(pos, n);

        if(m_data.size() > m_capacity){
            m_data.erase(--m_data.end());
        }
    }
    T top() {
        return *m_data.begin();
    }
    void remove(){
        m_data.erase(m_data.begin());
    }

private:
    std::vector<T> m_data;
    size_t m_capacity;
};


auto do_benchmark(size_t set_size, size_t num_samples, bool silent = false) {

    if(!silent) {
        std::cout << "QUEUE CAPACITY: " << set_size << " SAMPLES: " << num_samples << std::endl;
    }

    clock_t start, end;

    buffer_set<int> queue_set(set_size);
    start = clock();
    for(auto i = 0; i < num_samples; ++i){
        queue_set.insert(rand());
    }

    end = clock();
    auto time_set = end - start;
    if(!silent) {
        std::cout << "Set: " << std::endl;
        std::cout << 1000.0 * time_set / CLOCKS_PER_SEC << " ms" << std::endl;
    }

    buffer_vector<int> queue_vector(set_size);
    start = clock();
    for(auto i = 0; i < num_samples; ++i){
        queue_vector.insert(rand());
    }

    end = clock();
    auto time_vector = end - start;
    if(!silent) {
        std::cout << "Vector: " << std::endl;
        std::cout << 1000.0 * time_vector / CLOCKS_PER_SEC << " ms" << std::endl << std::endl;
    }

    return std::make_pair(time_set, time_vector);
}

void benchmark(){
    do_benchmark(5, 10000000);
    do_benchmark(10, 10000000);
    do_benchmark(200, 10000000);
    do_benchmark(1000, 10000000);

    size_t left = 100;
    size_t right = 200;
    while(left < right){
        auto center = left + (right - left) / 2;
        auto result = do_benchmark(center, 10000000, true);
        if(result.first > result.second){
            left = center;
        } else {
            right = center;
        }
    }

    std::cout << "Crossover: " << left << std::endl;
}

void test() {
    buffer_set<int> set(3);

    set.insert(4);
    set.insert(3);
    set.insert(5);
    set.insert(6);
    set.insert(6);
    assert(set.top() == 6);
    set.remove();
    assert(set.top() == 5);
    set.remove();
    assert(set.top() == 4);


    buffer_vector<int> vector(3);

    vector.insert(4);
    vector.insert(3);
    vector.insert(5);
    vector.insert(6);
    vector.insert(6);
    assert(vector.top() == 6);
    vector.remove();
    assert(vector.top() == 5);
    vector.remove();
    assert(vector.top() == 4);

    std::cout << "All tests passed!" << std::endl << std::endl;
}

int main() {
    test();
    benchmark();

    return 0;
}
