#ifndef PERFECT_CACHE
#define PERFECT_CACHE

#include <cstddef>
#include <unordered_map>
#include <list>
#include <deque>
#include <iterator>
#include <algorithm>

template <typename key_T, typename page_T, typename page_getter>
class perfect_cache
{
    std::size_t cache_capacity_;
    page_getter slow_get_page_;

    using iterator = typename std::list<page_T>::iterator;

    std::list<page_T> pages_;
    std::unordered_map<key_T, iterator> cache_storage_;
    std::deque<key_T> requests_;


     public:

        template<std::forward_iterator Iterator>
        perfect_cache(size_t capacity, page_getter slow_get_page, Iterator begin, Iterator end):
            cache_capacity_{capacity}, slow_get_page_{slow_get_page}, requests_{begin, end} {}

        bool lookup_update()
        {
            if (cache_storage_.empty()) throw "Cache storage is empty";

            auto key = std::move(requests_.front());
            requests_.pop_front();

            if (cache_storage_.contains(key)) return true;

            auto request_next_occurrence = find_next_occurrence(key);

            if (request_next_occurrence != no_later_occurrencies)
            {
                if (full())
                {
                    auto [iter, next_occurrence] = find_latest();

                    if (next_occurrence != no_later_occurrencies && next_occurrence < request_next_occurrence)
                        return false;
                    else
                        pages_.erase(iter->second);
                        cache_storage_.erase(iter);
                }

            auto iter = pages_.emplace_back(slow_get_page_(key));
            cache_storage_.emplace(key, iter);

            }

            return false;
        }

        bool full() const {return pages_.size() == cache_capacity_; }

    private:

        static constexpr int no_later_occurrencies = -1;

        int find_next_occurrence(const key_T& key)
        {
            auto iter = std::ranges::find(requests_, key);


            if (iter == requests_.end())
                return no_later_occurrencies;
            else
                return std::distance(requests_.begin(), iter) + 1;
        }

        std::pair<iterator, int> find_latest()
        {
            int latest_occurrence = 0;
            auto iter = cache_storage_.begin();
            auto latest_iter = iter;

            for (auto end_iter = cache_storage_.end(); iter != end_iter; ++iter)
            {
                int next_occurrence = find_next_occurrence(iter->first);

                if (next_occurrence == no_later_occurrencies)
                    return std::pair{iter, next_occurrence};
                else if (next_occurrence > latest_occurrence)
                {
                    latest_iter = iter;
                    latest_occurrence = next_occurrence;
                }
            }

            return std::pair{latest_iter, latest_occurrence};

        }

};

#endif