#include <list>
#include <stack>
#include <unordered_map>

template <typename key_T, typename page_T>
class cache
{
    enum class state
    {
        lir,
        resident_hir,
        non_resident_hir,
    };

    enum class location
    {
        in,
        out,
    };

    struct element
    {
        key_T key;
        state element_state;
        location element_location;
    };

    std::size_t Lhirs;
    std::size_t count_lirs;
    std::size_t Llirs;
    std::stack<page_T> lirs_stack;
    std::list<page_T> res_hir_collection;

    std::unordered_map<key_T, element> caсhe_storage;

};