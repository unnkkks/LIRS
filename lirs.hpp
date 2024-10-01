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
        page_T page;
        state element_state;
        location element_location;
    };

    std::size_t Lhirs;
    std::size_t count_lirs;
    std::size_t Llirs;

    std::stack<element> lirs_stack;

    std::list<element> resident_HIR_collection;

    std::unordered_map<key_T, element> cache_storage;

    void visit_LIR(element elem)
    {
        element front_elem;
        front_elem = lirs_stack.back();
        while(front_elem->element_state == lir)
        {
            elem_hit_the_top(front_elem);
            front_elem = lirs_stack.back();
        }
        stack_pruning(lirs_stack);
    }

    void stack_pruning(stack_)
    {
        element front_elem;
        front_elem = stack_.back()
        while(front_elem->element_state == resident_hir or front_elem->element_state = non_resident_hir)
        {
            remove_data_blocks(front_elem);
            front_elem = stack_.back();
        }

    }

    void elem_hit_the_top(element elem)
    {
        lirs_stack.remove(elem);
        lirs_stack.push_front(elem);
        //return???
    }
    void remove_data_blocks(element elem)
    {
        front_elem = lirs_stack.back();
        lirs_stack.pop_back();
        front_elem->element_state = out;
        //return??
    }

    void visit_resident_HIR(element elem)
    {
        element front_elem;
        front_elem = lirs_stack.back();
        while(front_elem->element_state != resident_hir)
        {
            add_to_res_HIR_collection(front_elem);
            remove_data_blocks(front_elem);
            front_elem = lirs_stack.back();
        }

        elem_hit_the_top(front_elem);

        if(lirs_stack.contains(front_elem))
        {
            front_elem->element_state = lir;
            front_elem = lirs_stack.back();
            resident_HIR_collection.delete(front_elem);
        }

        stack_pruning(lirs_stack);
        else
        {
            list_top = resident_HIR_collection.front();
            resident_HIR_collection.push_back(list_top);
        }
    }

    void add_to_res_HIR_collection(element elem)
    {
        resident_HIR_collection.push_back(elem);
        elem->element_state = resident_hir;
    }


};
