/// Part of PB173 homework, created by Ondřej Budai <ondrej@budai.cz>

#include "hash_set_linked_list.hh"
#include "hash_set_linear_probing.hh"

#include <iostream>
#include <cassert>
#include <ctime>
#include <unordered_set>
#include <set>
#include <random>
#include <iomanip>

class int_container {
public:
    explicit int_container(int value_) : value{value_}{ptr = new int; std::cout << "allocated" << std::endl;}
    ~int_container(){
        delete ptr; std::cout << "freed" << std::endl;
    }
    int_container(const int_container& other){
        ptr = new int(*other.ptr);
        value = other.value;
        std::cout << "copied" << std::endl;
    }
    bool operator==(const int_container& other){ return value == other.value;}
    int value;
    int* ptr;
};

namespace std {
    template<> struct hash<int_container>{
        std::size_t operator()(const int_container& k) const {
            return static_cast<std::size_t>(k.value);
        }
    };
}


constexpr int INT_ITERATIONS = 5000000;
constexpr int STRING_ITERATIONS = 500000;
constexpr int STRING_LENGTH = 64;

/// Generates random string of corresponding length
/// Originally from https://stackoverflow.com/questions/21842849/how-to-generate-a-random-string-in-c
/// Modified
std::string generate_random_string(size_t length){


    const std::string VALID_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::uniform_int_distribution<size_t> distribution(0, VALID_CHARS.size() - 1);
    static std::default_random_engine random_engine;
    std::string random_string;
    std::generate_n(std::back_inserter(random_string), length, [&]()
    {
        return VALID_CHARS[distribution(random_engine)];
    });

    return random_string;
}

template<typename Set> void generic_test_int() {
    Set set;

    set.insert(5);
    assert(set.find(5));
    assert(!set.find(0));
    set.insert(0);
    assert(set.find(0));
    assert(!set.find(1));

    Set set2;
    for(int i = 0; i < 2048; ++i){
        set2.insert(i);
        assert(set2.find(i));
    }
    for(int i = 0; i < 2048; ++i){
        assert(set2.find(i));
    }
    assert(!set2.find(2048));
}


template<typename Set> void generic_test_string() {
    Set set;

    set.insert("test");
    assert(set.find("test"));
    assert(!set.find(""));
    set.insert("");
    assert(set.find(""));
    assert(!set.find("kocka"));

    Set set2;
    std::vector<std::string> vector;

    for(auto i = 0; i < 2048; ++i){
        auto str = generate_random_string(10);
        set.insert(str);
        assert(set.find(str));
    }

    for(const auto& str: vector){
        assert(set.find(str));
    }
}

void test() {
    std::cout << "Testing hash table using linked lists." << std::endl;
    generic_test_int<hash_set_linked_list<int>>();
    generic_test_string<hash_set_linked_list<std::string>>();
    std::cout << "Testing hash table using linear probing." << std::endl;
    generic_test_int<hash_set_linear_probing<int>>();
    generic_test_string<hash_set_linear_probing<std::string>>();

    std::cout << "Tests successfully ran." << std::endl << std::endl;
}

template<typename Set> void generic_benchmark_insert(Set& set){
    auto start = clock();
    for(auto i = 0; i < INT_ITERATIONS; ++i) {
        set.insert(rand());
    }
    auto end = clock();
    auto time = end - start;
    std::cout << "Inserting random numbers: " << (1000.0 * time / CLOCKS_PER_SEC) << "ms" << std::endl;
}

template<typename Set> void generic_benchmark_find(Set& set){
    auto start = clock();
    for(auto i = 0; i < INT_ITERATIONS; ++i) {
        set.find(rand());
    }
    auto end = clock();
    auto time = end - start;
    std::cout << "Searching for random numbers: " << (1000.0 * time / CLOCKS_PER_SEC) << "ms" << std::endl;
}


template<typename Set> void generic_benchmark_int(){
    Set set_random;
    generic_benchmark_insert(set_random);
    generic_benchmark_find(set_random);

}

template<typename Set> void generic_benchmark_string_insert(Set& set, std::vector<std::string>& vector){
    auto start = clock();
    for(int i = 0; i < STRING_ITERATIONS; ++i){
        auto str = generate_random_string(STRING_LENGTH);
        vector.emplace_back(str);
        set.insert(str);
    }
    auto end = clock();
    auto time = end - start;
    std::cout << "Inserting random strings: " << (1000.0 * time / CLOCKS_PER_SEC) << "ms" << std::endl;
}

template<typename Set> void generic_benchmark_string_find_included(Set& set, std::vector<std::string>& vector){
    auto start = clock();
    for(const auto& str: vector){
        set.find(str);
    }
    auto end = clock();
    auto time = end - start;
    std::cout << "Searching for random strings contained in set: " << (1000.0 * time / CLOCKS_PER_SEC) << "ms" << std::endl;
}

template<typename Set> void generic_benchmark_string_find_not_included(Set& set){
    auto start = clock();
    for(int i = 0; i < STRING_ITERATIONS; ++i){
        auto str = generate_random_string(STRING_LENGTH);
        set.find(str);
    }
    auto end = clock();
    auto time = end - start;
    std::cout << "Searching for random strings not contained in set: " << (1000.0 * time / CLOCKS_PER_SEC) << "ms" << std::endl;
}

template<typename Set> void generic_benchmark_string(){
    Set set;
    std::vector<std::string> generated_strings;
    generic_benchmark_string_insert(set, generated_strings);
    generic_benchmark_string_find_included(set, generated_strings);
    generic_benchmark_string_find_not_included(set);

}

void benchmark() {
    std::cout << "Benchmarking hash table with linked list:" << std::endl;
    std::cout << "=========================================" << std::endl;
    generic_benchmark_int<hash_set_linked_list<int>>();
    generic_benchmark_string<hash_set_linked_list<std::string>>();
    std::cout << std::endl;

    std::cout << "Benchmarking hash table with linear probing:" << std::endl;
    std::cout << "============================================" << std::endl;
    generic_benchmark_int<hash_set_linear_probing<int>>();
    generic_benchmark_string<hash_set_linear_probing<std::string>>();
    std::cout << std::endl;

    std::cout << "Benchmarking std::unordered_set:" << std::endl;
    std::cout << "================================" << std::endl;
    generic_benchmark_int<std::unordered_set<int>>();
    generic_benchmark_string<std::unordered_set<std::string>>();
    std::cout << std::endl;

    std::cout << "Benchmarking std::set:" << std::endl;
    std::cout << "======================" << std::endl;
    generic_benchmark_int<std::set<int>>();
    generic_benchmark_string<std::set<std::string>>();
    std::cout << std::endl;
}

int main() {

//    hash_set_linear_probing<int_container> malformed_container;
//    malformed_container.insert(int_container(1));
//    generic_test_string<hash_set_linear_probing<std::string>>();
//    return 0;
    // Set precision of floats to zero, e.g. 1024.42 => 1024
    std::cout << std::fixed << std::setprecision(0) << std::endl;
    // Run tests and then benchmark those structures
    test();
    benchmark();
    return 0;
}
