#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>

int main(int argc, char* argv[]) {
    int nth_prime = std::atoi(argv[1]);

    std::vector<bool> array;

    array.resize(static_cast<unsigned long>(nth_prime * std::log(nth_prime) + nth_prime * std::log(std::log(nth_prime))), false);

    int current = 2;
    int primes_found = 0;

    while(true){
        if(array[current] == true){
            current++;
            continue;
        }
        primes_found++;

        if(primes_found == nth_prime){
            std::cout << current << std::endl;
            return 0;
        }

        int cur_current = current * 2;
        while(cur_current < array.size()){
            array[cur_current] = true;
            cur_current += current;
        }

        current++;
    }
}
