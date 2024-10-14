#include <list>
#include <unordered_map>
#include <cstddef>
#include <iostream>

template <typename key_T, typename page_T, typename page_getter>
class lirs
{
    enum class State
    {
        lir,
        resident_hir,
        non_resident_hir,
    };

    enum class Location
    {
        in_stack,
        in_list,
        out,
    };

    struct element
    {
        key_T key;
        page_T page;
        State state;
        Location location;
    };

    page_getter slow_get_page(key_T key) {return key;};

    std::size_t Lhirs;
    std::size_t Llirs;

    std::list<element> lirs_stack;
    std::list<element> resident_HIR_collection;

    std::unordered_map<key_T, element> cache_storage;

    public:

        lirs(std::size_t cache_capacity, page_getter slow_get_page)
        {
            if (cache_capacity > 2)
            {
                Lhirs = static_cast<size_t>(cache_capacity * 0.4) ;
                Llirs = cache_capacity - Lhirs;
            }
            else if (cache_capacity == 2)
            {
                Llirs = 1;
                Lhirs = 1;
            }
            else
            {
                throw "Incorrect cache capacity";
            }
        }

        bool lookup_update(key_T key)
        {
            auto elem = cache_storage.find(key);

            if (elem == cache_storage.end())
            {
                if (full())
                {
                    cache_storage.erase(lirs_stack.back().key);
                    lirs_stack.pop_back();
                }

                lirs_stack.push_front(elem->second);
                cache_storage.insert({key, {key, key, State::resident_hir, Location::in_stack}});

                renew_hir_cache(elem->second);

                return false;
            }

            else
            {
                State state_of_elem = elem->second.state;

                if (state_of_elem == State::lir) { visit_LIR(elem->second); }
                else if (state_of_elem == State::resident_hir) { visit_resident_HIR(elem->second); }
                else { visit_non_resident_HIR(elem->second);}

                return true;
            }
        }

        bool full() const {return (lirs_stack.size() == Llirs); }

    private:

        void renew_hir_cache (const element& elem)
        {
            if (resident_HIR_collection.size() < Lhirs)
            {
                resident_HIR_collection.push_front(elem);
                cache_storage[elem.key].location = Location::in_list;
            }

            element& first_hir = resident_HIR_collection.back();
            Location is_in_stack = cache_storage[first_hir.key].location;

            if (is_in_stack == Location::in_stack)
            {
                cache_storage[first_hir.key].state = State::non_resident_hir;
            }
            else
            {
                cache_storage.erase(first_hir.key);
            }

            resident_HIR_collection.pop_back();
            resident_HIR_collection.push_front(elem);
            cache_storage[elem.key].location = Location::in_list;
        }


        void visit_LIR(const element& elem)
        {
            lirs_stack.push_front(elem);
            stack_pruning();
        }

        void stack_pruning()
        {
            element& front_elem = lirs_stack.front();
            while(front_elem.state != State::lir)
            {
                remove_data_blocks();
                front_elem = lirs_stack.front();
            }

        }

        void remove_data_blocks()
        {
            element& front_elem = lirs_stack.front();
            lirs_stack.pop_back();
            front_elem.location = Location::out;
        }

        void visit_resident_HIR(const element& elem)
        {

            if (elem.location == Location::in_stack)
            {
                element& front_elem = lirs_stack.front();
                front_elem.state = State::lir;
                resident_HIR_collection.erase(lirs_stack.begin());
                stack_pruning();
            }
            else
            {
                element& list_top = resident_HIR_collection.front();
                resident_HIR_collection.push_back(list_top);
                list_top.location = Location::in_stack;
            }
        }

        void visit_non_resident_HIR(const element& elem)
        {
            element& front_elem = resident_HIR_collection.front();
            resident_HIR_collection.pop_front();
            front_elem.state = State::non_resident_hir;

            if (elem.location == Location::in_stack)
            {
                elem.state = State::lir;
                element& stack_bottom = lirs_stack.back();

                resident_HIR_collection.push_back(stack_bottom);
                stack_bottom.state = State::resident_hir;
                stack_bottom.location = Location::in_list;
            }
            else
            {
                resident_HIR_collection.push_back(elem);
            }
        }

    };
