#include "lirs.hpp"
#include <iostream>


int slow_get_page(int key) {return key;}

int main()
{
    std::size_t cache_capacity = 0;
    int num_of_elems = 0;
    std::size_t cache_len = 0;

    std::cin >> cache_len;

    cache<int, int, int(&)(int)> cache(cache_capacity, slow_get_page);

    if (!(std::cin.good()) || cache_len < 0)
    {
        std::cerr << "Incorrect input of the cache length\n";
    }

    if (!(std::cin.good()) || num_of_elems < 0)
    {
        std::cerr << "Incorrect input of the number of elements\n";
    }

    for (int i = 0; i < num_of_elems; i++)
    {
        int key;
        std::cin >> key;
        cache.lookup_update(key);
    }

}