#include <bitset>

struct OB
{
    using internal_set = std::bitset<256>;
    OB() = default;
    OB(const internal_set& set_) : set{set_}{}
    internal_set set{};

    void insert( char c ) {
        set.set(c);
    }
    void erase( char c ) {
        set.reset(c);
    }
    int count( char c ) {
        return const_cast<const internal_set>(set)[c];
    }

    OB operator&( const OB &o ) {
        return set & o.set;

    }
    OB operator|( const OB &o ) {
        return set | o.set;
    }
};
