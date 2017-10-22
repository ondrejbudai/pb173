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
    explicit int_container(int value_) : value{value_}{ptr = new int;}
    ~int_container(){
        delete ptr;
    }
    int_container(const int_container& other){
        ptr = new int(*other.ptr);
        value = other.value;
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

template<template<typename ...> typename Set> void generic_test_int() {
    Set<int> set;

    set.insert(5);
    assert(set.find(5));
    assert(!set.find(0));
    set.insert(0);
    assert(set.find(0));
    assert(!set.find(1));

    Set<int> set2;
    for(int i = 0; i < 2048; ++i){
        set2.insert(i);
        assert(set2.find(i));
    }
    for(int i = 0; i < 2048; ++i){
        assert(set2.find(i));
    }
    assert(!set2.find(2048));

    Set<int> set_erase;
    assert(!set_erase.find(2));

    set_erase.erase(2);
    assert(!set_erase.find(2));

    set_erase.insert(2);
    assert(set_erase.find(2));

    set_erase.erase(2);
    assert(!set_erase.find(2));

    set_erase.erase(2);
    assert(!set_erase.find(2));

    set_erase.insert(2);
    assert(set_erase.find(2));

    std::vector<int> numbers;
    Set<int> set3;
    for(int i = 0; i < 8096; ++i){
        numbers.emplace_back(i);
        set3.insert(i);
        assert(set3.find(i));
    }

    std::shuffle(std::begin(numbers), std::end(numbers), std::mt19937{});

    for(const auto& n: numbers){
        assert(set3.find(n));
        set3.erase(n);
        assert(!set3.find(n));
    }
    for(const auto& n: numbers){
        assert(!set3.find(n));
    }
    for(int i = 0; i < 8096; ++i){
        assert(!set3.find(i));
        set3.insert(i);
        assert(set3.find(i));
    }
    for(const auto& n: numbers){
        assert(set3.find(n));
    }
}


template<template<typename ...> typename Set> void generic_test_string() {
    Set<std::string> set;

    set.insert("test");
    assert(set.find("test"));
    assert(!set.find(""));
    set.insert("");
    assert(set.find(""));
    assert(!set.find("kocka"));

    Set<std::string> set2;
    std::vector<std::string> vector;

    for(auto i = 0; i < 2048; ++i){
        auto str = generate_random_string(64);
        set.insert(str);
        assert(set.find(str));
    }

    for(const auto& str: vector){
        assert(set.find(str));
    }
}

void test() {
    std::cout << "Testing hash table using linked lists." << std::endl;
    generic_test_int<hash_set_linked_list>();
    generic_test_string<hash_set_linked_list>();
    std::cout << "Testing hash table using linear probing." << std::endl;
    generic_test_int<hash_set_linear_probing>();
    generic_test_string<hash_set_linear_probing>();

    std::cout << "Tests successfully ran." << std::endl << std::endl;
}

template<typename IntSet> void generic_benchmark_insert(IntSet& set){
    auto start = clock();
    for(auto i = 0; i < INT_ITERATIONS; ++i) {
        set.insert(rand());
    }
    auto end = clock();
    auto time = end - start;
    std::cout << "Inserting random numbers: " << (1000.0 * time / CLOCKS_PER_SEC) << "ms" << std::endl;
}

template<typename IntSet> void generic_benchmark_find(IntSet& set){
    auto start = clock();
    for(auto i = 0; i < INT_ITERATIONS; ++i) {
        set.find(rand());
    }
    auto end = clock();
    auto time = end - start;
    std::cout << "Searching for random numbers: " << (1000.0 * time / CLOCKS_PER_SEC) << "ms" << std::endl;
}


template<template<typename ...> typename Set> void generic_benchmark_int(){
    Set<int> set_random;
    generic_benchmark_insert(set_random);
    generic_benchmark_find(set_random);

}

template<typename StringSet> void generic_benchmark_string_insert(StringSet& set, std::vector<std::string>& vector){
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

template<typename StringSet> void generic_benchmark_string_find_included(StringSet& set, std::vector<std::string>& vector){
    auto start = clock();
    for(const auto& str: vector){
        set.find(str);
    }
    auto end = clock();
    auto time = end - start;
    std::cout << "Searching for random strings contained in set: " << (1000.0 * time / CLOCKS_PER_SEC) << "ms" << std::endl;
}

template<typename StringSet> void generic_benchmark_string_find_not_included(StringSet& set){
    auto start = clock();
    for(int i = 0; i < STRING_ITERATIONS; ++i){
        auto str = generate_random_string(STRING_LENGTH);
        set.find(str);
    }
    auto end = clock();
    auto time = end - start;
    std::cout << "Searching for random strings not contained in set: " << (1000.0 * time / CLOCKS_PER_SEC) << "ms" << std::endl;
}

template<template<typename ...> typename Set> void generic_benchmark_string(){
    Set<std::string> set;
    std::vector<std::string> generated_strings;
    generic_benchmark_string_insert(set, generated_strings);
    generic_benchmark_string_find_included(set, generated_strings);
    generic_benchmark_string_find_not_included(set);

}

void benchmark() {
    std::cout << "Benchmarking hash table with linked list:" << std::endl;
    std::cout << "=========================================" << std::endl;
    generic_benchmark_int<hash_set_linked_list>();
    generic_benchmark_string<hash_set_linked_list>();
    std::cout << std::endl;

    std::cout << "Benchmarking hash table with linear probing:" << std::endl;
    std::cout << "============================================" << std::endl;
    generic_benchmark_int<hash_set_linear_probing>();
    generic_benchmark_string<hash_set_linear_probing>();
    std::cout << std::endl;

    std::cout << "Benchmarking std::unordered_set:" << std::endl;
    std::cout << "================================" << std::endl;
    generic_benchmark_int<std::unordered_set>();
    generic_benchmark_string<std::unordered_set>();
    std::cout << std::endl;

    std::cout << "Benchmarking std::set:" << std::endl;
    std::cout << "======================" << std::endl;
    generic_benchmark_int<std::set>();
    generic_benchmark_string<std::set>();
    std::cout << std::endl;
}

int main() {
    // Set precision of floats to zero, e.g. 1024.42 => 1024
    std::cout << std::fixed << std::setprecision(0) << std::endl;
    // Run tests and then benchmark those structures
    test();
    benchmark();
    return 0;
}
