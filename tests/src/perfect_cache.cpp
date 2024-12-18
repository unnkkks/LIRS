#include "perfect_cache.hpp"
#include <iostream>
#include <cstddef>
#include <vector>

int slow_get_page(int key) {return key;}

int main()
{

    std::size_t num_of_elems = 0;
    std::size_t cache_len = 0;

    std::cin >> num_of_elems;

    if (!std::cin.good())
    {
        std::cerr << "Incorrect input of the number of the elements\n";
        return 1;
    }

    std::cin >> cache_len;

    if (!std::cin.good())
    {
        std::cerr << "Incorrect input of the cache length\n";
        return 1;
    }

    std::vector<int> elements;
    elements.reserve(num_of_elems);
    for (size_t i = 0; i != num_of_elems; ++i)
    {
        int key;
        std::cin >> key;
        if (std::cin.fail())
        {
            std::cerr << "Incorrect input of the key\n";
            return 1;
        }
        elements.push_back(key);
    }

    perfect_cache<int, int, int(*)(int)> cache(cache_len, slow_get_page, elements.begin(), elements.end());
    std::size_t hits = 0;

    for (size_t i = 0; i < num_of_elems; ++i)
    {
        hits += cache.lookup_update();
    }

    std::cout << hits << std::endl;

}