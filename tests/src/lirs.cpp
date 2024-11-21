#include "lirs.hpp"
#include <iostream>
#include <cstddef>

int slow_get_page(int key) {return key;}

int main()
{

    std::size_t num_of_elems = 0;
    std::size_t cache_len = 0;

    std::cin >> cache_len >> num_of_elems;

    if (!std::cin.good())
    {
        std::cerr << "Incorrect input of the cache length\n";
        return 1;
    }

    if (!std::cin.good())
    {
        std::cerr << "Incorrect input of the number of elements\n";
        return 1;
    }

    lirs<int, int, int(*)(int)> cache(cache_len, slow_get_page);
    std::size_t hits = 0;

    for (int i = 0; i < num_of_elems; ++i)
    {
        int key;
        std::cin >> key;

        if (!std::cin.good())
        {
            std::cerr << "Incorrect input of the key\n";
            return 1;
        }

        hits += cache.lookup_update(key);

    }

    std::cout << hits << std::endl;

}