#include "lirs.hpp"
#include <iostream>


int slow_get_page(int key) {return key;}

int main()
{

    int num_of_elems = 0;
    std::size_t cache_len = 0;
    int hits = 0;

    std::cin >> cache_len >> num_of_elems;

    if (!std::cin.good() || cache_len < 0)
    {
        std::cerr << "Incorrect input of the cache length\n";
        return 1;
    }

    if (!std::cin.good() || num_of_elems < 0)
    {
        std::cerr << "Incorrect input of the number of elements\n";
        return 1;
    }

    lirs<int, int, int(&)(int)> cache(cache_len, slow_get_page);

    for (int i = 0; i < num_of_elems; i++)
    {
        int key;
        std::cin >> key;

        if (!std::cin.good())
        {
            std::cerr << "Incorrect input of the key\n";
            return 1;
        }

        if (cache.lookup_update(key))
        {
            hits += 1;
        }
    }

    std::cout << "Number of hits:" << hits << std::endl;

}