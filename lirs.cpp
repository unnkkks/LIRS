#include "lirs.hpp"

int main()
{
    size_t cache_capacity = 0;
    int num_of_elems = 0;

    std::cin >> cache_len;

    cache lirs<key_T, page_T>;

    page_T slow_get_page(key_T key) {return key;}

    if (!(std::cin.good()) || cache_len < 0)
    {
        std::cerr << "Incorrect input of the cache length";
    }

    if (!(std::cin.good()) || num_of_elems < 0)
    {
        std::cerr << "Incorrect input of the number of elements";
    }

    for (int i = 0; i < num_of_elems; i++)
    {
        key_T key;
        std::cin >> key;
        lookup_update(key);
    }

}