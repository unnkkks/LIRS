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
        in_stack,
        in_list,
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
    std::size_t Llirs;

    std::list<element> lirs_stack;
    std::list<element> resident_HIR_collection;

    std::unordered_map<key_T, element> cache_storage;

    public:

        cache(size_t cache_capacity, slow_get_page page)
        {
            if (cache_capacity > 2)
            {
                Lhirs = static_cast<size_t>(cache_capacity * 0.4) ;
                Llirs = cache_capacity - Lhirs;
            }
            else if (cache_capacity < 2)
            {
                std::err << "Incorrect cache capacity";
                throw;
            }
            else
            {
                Llirs = 1;
                Lhirs = 1;
            }
        }

        void lookup_update(key_T key, page_T slow_get_page)
        {
            auto hit = cache_storage.find(key);

            if (hit == cache_storage.end())
            {
                if (full())
                {
                    cache_storage.erase(lirs_stack.back().first);
                    lirs_stack.pop_back();
                }

                lirs_stack.emplace_front(key, slow_get_page);
                cache_storage.emplace(key, lirs_stack.begin());
            }
        }

        bool full() const {return (lirs_stack.size() == Llirs); }

    private:

        void visit_LIR(element* elem)
        {
            lirs_stack.push_front(elem);
            stack_pruning();
        }

        void stack_pruning()
        {
            element front_elem = lirs_stack.front();
            while(front_elem->element_state != state::lir)
            {
                remove_data_blocks();
                front_elem = lirs_stack.front();
            }

        }

        void remove_data_blocks()
        {
            element front_elem = lirs_stack.front();
            lirs_stack.pop_back();
            front_elem->element_state = location::out;
        }

        void visit_resident_HIR(element* elem)
        {

            if (elem->element_location == location::in_stack)
            {
                element front_elem = lirs_stack.front();
                front_elem->element_state = state::lir;
                resident_HIR_collection.erase(front_elem);
                stack_pruning();
            }
            else
            {
                element list_top = resident_HIR_collection.front();
                resident_HIR_collection.push_back(list_top);
                list_top->location = location::in_stack;
            }
        }

        void visit_non_resident_HIR(element* elem)
        {
            element front_elem = resident_HIR_collection.front();
            resident_HIR_collection.pop_front();
            front_elem->element_state = state::non_resident_hir;

            if (elem->element_location == location::in_stack)
            {
                elem->element_state = state::lir;
                element stack_bottom = lirs_stack.back();

                resident_HIR_collection.push_back(stack_bottom);
                stack_bottom->element_state = state::hir;
                stack_bottom->element_location = location::in_list;
            }
            else
            {
                resident_HIR_collection.push_back(elem);
            }
        }

    };
