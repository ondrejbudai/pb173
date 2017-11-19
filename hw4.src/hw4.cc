#define BRICK_BENCHMARK_REG
#define BRICK_BENCHMARK_MAIN

#include <brick-benchmark>
#include "int16_set_trie.hh"
#include "int16_set_bitvector.hh"

using namespace brick;

struct Set : benchmark::Group
{
    std::mt19937 rng;
    int16_set_bitvector set_bitvector;
    int16_set_bitvector set_bitvector2;
    int16_set_bitvector set_bitvector3;
    int16_set_trie set_trie;
    int16_set_trie set_trie2;
    int16_set_trie set_trie3;
    Set()
    {
        x.type = benchmark::Axis::Quantitative;
        x.name = "items";
        x.min = 1024;
        x.max = 65536 / 2;
        x.step = 2;
        x.log = true;

        y.type = benchmark::Axis::Qualitative;
        y.name = "implementation";
        y.min = 1;
        y.max = 2;
        y._render = []( int i )
        {
            switch ( i )
            {
                case 1: return "trie";
                case 2: return "bitfield";
            }
        };


    }

    void setup(int p, int q) override {
        benchmark::Group::setup(p, q);

        rng.seed(0);

        auto insert_func = [p = p / 2, &rng = rng](auto& set){
            for(auto i = 0; i < p; ++i){
                set.insert(rng());
            }
        };

        switch(q){
            case 1:
                set_trie = int16_set_trie{};
                insert_func(set_trie);

                set_trie2 = int16_set_trie{};
                insert_func(set_trie2);
                break;
            case 2:
                set_bitvector = int16_set_bitvector{};
                insert_func(set_trie);

                set_bitvector2 = int16_set_bitvector{};
                insert_func(set_bitvector2);
                break;
        }

        rng.seed(0);
    }



    std::string describe() override { return "category:set"; }

    BENCHMARK(insert)
    {
        switch ( q )
        {
            case 1: insert<int16_set_trie>(); break;
            case 2: insert<int16_set_bitvector>(); break;
        }
    }

    template <typename Set>
    void insert(){
        Set set;
        for(int i = 0; i < p; ++i){
            set.insert(rng());
        }
    }

    BENCHMARK(contains)
    {
        switch ( q )
        {
            case 1: contains(set_trie); break;
            case 2: contains(set_bitvector); break;
        }
    }

    template <typename Set>
    void contains(const Set& set){
        for(int i = 0; i < p; ++i){
            set.contains(rng());
        }
    }

    BENCHMARK(unionbench){
        switch ( q )
        {
            case 1: unionbench(set_trie3, set_trie, set_trie2); break;
            case 2: unionbench(set_bitvector3, set_bitvector, set_bitvector2); break;
        }
    }


    template <typename Set>
    void unionbench(Set& target_set, const Set& set, const Set& set2){
        target_set = set | set2;

   }
};
