#include <list>
#include <stack>
#include <unordered_map>

class cash
{
    enum state
    {
        lir,
        resident_hir,
        non_resident_hir,
    };

    enum location
    {
        in = 1,
        out = 0,
    };

    template <typename T>
    struct element
    {
        T key;
        state element_state;
        location element_location;
    };

    std::size_t Lhirs;
    std::size_t count_lirs;
    std::size_t Llirs;
    std::stack<int> lirs_stack;
    std::list<int> res_hir_collection;

    std::unordered_map<T, element<T>> cashe_storage;

}