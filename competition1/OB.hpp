#include <bitset>

struct OB
{
    using internal_set = std::bitset<256>;
    OB() = default;
    OB(const internal_set& set_) : set{set_}{}
    internal_set set{};

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
