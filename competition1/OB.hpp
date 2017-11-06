#include <bitset>

struct OB
{
    OB() = default;
    OB(const std::bitset<256>& set_) : set{set_}{}
    std::bitset<256> set{};
    
    void insert( char c ) {
        set[c] = true;
    }
    void erase( char c ) {
        set[c] = false;
    }
    int count( char c ) {
        return set[c];
    }

    OB operator&( const OB &o ) {
        return set & o.set;

    }
    OB operator|( const OB &o ) {
        return set | o.set;
    }
};
