#include "hash_set_linked_list.hh"
#include "hash_set_linear_probing.hh"


int main(){}




#define BRICK_BENCHMARK_REG
//#define BRICK_BENCHMARK_MAIN

/* ./bench category:stack | gnuplot > soubor.pdf */

//#include <brick-benchmark>
//
//using namespace brick;
//
//struct Stack : benchmark::Group
//{
//    static constexpr int items = 100000;
//    std::list<int> m_list;
//    std::vector<int> m_vector;
//    Stack()
//    {
//        x.type = benchmark::Axis::Quantitative;
//        x.name = "items";
//        x.min = 1;
//        x.max = 65536;
//        x.step = 16;
//        x.log = true;
//
//        y.type = benchmark::Axis::Qualitative;
//        y.name = "implementation";
//        y.min = 1;
//        y.max = 1;
//        y._render = []( int i )
//        {
//            switch ( i )
//            {
//                case 1: return "list";
//                case 2: return "vector";
//            }
//        };
//
//
//    }
//
//    void setup(int p, int q) override {
//        benchmark::Group::setup(p, q);
//
//        m_list.clear();
//        m_vector.clear();
//        for(auto i = 0; i < p; ++i){
//            m_list.push_back(i);
//            m_vector.push_back(i);
//        }
//
//        static std::mt19937 random;
//
//        m_list_iterator = std::begin(m_list);
//
//        for(auto i = 0; i < random() % p; ++i){
//            ++m_list_iterator;
//        }
//    }
//
//
//
//    std::string describe() { return "category:stack"; }
//
//    BENCHMARK( read )
//    {
//        switch ( q )
//        {
//            case 1: read_list(); break;
//            case 2: read_list(); break;
//        }
//    }
//
//    double normal() override {
//        return 1.0;
//    }
//
//    void read_list(){
//        int len = 1024;
//        auto a = 0;
//        for(auto i = 0; i < len; ++i){
//
//        }
//    }
//
//    std::list<int>::iterator m_list_iterator;
//};


