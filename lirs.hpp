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

    std::stack<element> lirs_stack;

    std::list<element> resident_HIR_collection;

    std::unordered_map<key_T, element> cache_storage;

};

void visit_LIR(struct elem);
void elem_hit_the_top(struct elem);
void remove_data_blocks(struct elem);
void visit_resident_HIR(struct elem);
void access_not_in_LIR_and_HIR_data(struct elem);
void visit_non_resident_HIR(struct elem);
