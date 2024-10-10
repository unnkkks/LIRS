#include <list>
#include <stack>
#include <unordered_map>
#include <cstddef>
#include <iostream>

template <typename key_T, typename page_T, typename page_getter>
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

        cache(std::size_t cache_capacity, page_getter slow_get_page)
        {
            if (cache_capacity > 2)
            {
                Lhirs = static_cast<size_t>(cache_capacity * 0.4) ;
                Llirs = cache_capacity - Lhirs;
            }
            else if (cache_capacity < 2)
            {
                std::cerr << "Incorrect cache capacity\n";
                throw;
            }
            else
            {
                Llirs = 1;
                Lhirs = 1;
            }
        }

        bool lookup_update(key_T key)
        {
            if (cache_storage.find(key) == cache_storage.end())
            {
                auto elem = cache_storage.find(key);
                if (elem->second.element_state == state::lir)
                {
                    visit_LIR(elem->second);
                }

                else if (elem->second.element_state == state::resident_hir)
                {
                    visit_resident_HIR(elem->second);
                }

                else
                {
                    visit_non_resident_HIR(elem->second);
                }

                return true;
            }

            else
            {
                if (full())
                {
                    cache_storage.erase(lirs_stack.back().key);
                    lirs_stack.pop_back();
                }

                auto elem = cache_storage.find(key);
                lirs_stack.push_front(elem->second);
                cache_storage.insert({key, {key, key, state::resident_hir, location::in_stack}});

                renew_hir_cache(elem->second);

                return false;
            }
        }

        bool full() const {return (lirs_stack.size() == Llirs); }

    private:

        void renew_hir_cache (element elem)
        {
            if (resident_HIR_collection.size() < Lhirs)
            {
                resident_HIR_collection.push_front(elem);
                cache_storage[elem.key].element_location = location::in_list;
            }

            element first_hir = resident_HIR_collection.back();
            location is_in_stack = cache_storage[first_hir.key].element_location;

            if (is_in_stack == location::in_stack)
            {
                cache_storage[first_hir.key].element_state = state::non_resident_hir;
            }
            else
            {
                cache_storage.erase(first_hir.key);
            }

            resident_HIR_collection.pop_back();
            resident_HIR_collection.push_front(elem);
            cache_storage[elem.key].element_location = location::in_list;
        }


        void visit_LIR(element elem)
        {
            lirs_stack.push_front(elem);
            stack_pruning();
        }

        void stack_pruning()
        {
            element front_elem = lirs_stack.front();
            while(front_elem.element_state != state::lir)
            {
                remove_data_blocks();
                front_elem = lirs_stack.front();
            }

        }

        void remove_data_blocks()
        {
            element front_elem = lirs_stack.front();
            lirs_stack.pop_back();
            front_elem.element_location = location::out;
        }

        void visit_resident_HIR(element elem)
        {

            if (elem.element_location == location::in_stack)
            {
                element front_elem = lirs_stack.front();
                front_elem.element_state = state::lir;
                resident_HIR_collection.erase(lirs_stack.begin());
                stack_pruning();
            }
            else
            {
                element list_top = resident_HIR_collection.front();
                resident_HIR_collection.push_back(list_top);
                list_top.element_location = location::in_stack;
            }
        }

        void visit_non_resident_HIR(element& elem)
        {
            element front_elem = resident_HIR_collection.front();
            resident_HIR_collection.pop_front();
            front_elem.element_state = state::non_resident_hir;

            if (elem.element_location == location::in_stack)
            {
                elem.element_state = state::lir;
                element stack_bottom = lirs_stack.back();

                resident_HIR_collection.push_back(stack_bottom);
                stack_bottom.element_state = state::resident_hir;
                stack_bottom.element_location = location::in_list;
            }
            else
            {
                resident_HIR_collection.push_back(elem);
            }
        }

    };
