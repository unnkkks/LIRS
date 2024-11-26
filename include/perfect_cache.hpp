#ifndef PERFECT_CACHE
#define PERFECT_CACHE

#include <cstddef>
#include <unordered_map>
#include <list>
#include <deque>
#include <iterator>
#include <algorithm>
#include <iostream>


template <typename key_T>
class occurrence_table
{
    std::unordered_map<key_T, std::deque<int>> table_;

    public:
            static constexpr int no_next = -1;

            template<typename key_iter>
            occurrence_table(key_iter begin, key_iter end)
            {
                std::for_each(begin, end,
                [this, index = 0](const key_T &key) mutable{add(key, index++);});
            }

            void add(const key_T &key, int occurrence) {table_[key].push_back(occurrence);}

            void pop_first(const key_T &key) {table_.at(key).pop_front();}

            int first(const key_T &key) const
            {
                auto &deque = table_.at(key);
                return deque.empty() ? no_next : deque.front();
            }

};


template <typename key_T, typename page_T, typename page_getter>
class perfect_cache
{
    std::size_t cache_capacity_;
    page_getter slow_get_page_;

    using iterator = typename std::list<page_T>::iterator;

    std::list<page_T> pages_;
    std::unordered_map<key_T, iterator> cache_storage_;
    std::deque<key_T> requests_;
    occurrence_table<key_T> occurrence_table_;


     public:

        template<std::forward_iterator Iterator>
        perfect_cache(size_t capacity, page_getter slow_get_page, Iterator begin, Iterator end):
            cache_capacity_{capacity}, slow_get_page_{slow_get_page}, requests_{begin, end}, occurrence_table_{begin, end}
            {
                for (size_t i = 0; i < std::min(capacity, requests_.size()); ++i){
                    pages_.emplace_back(slow_get_page_(requests_[i]));
                    cache_storage_.emplace(requests_[i], std::prev(pages_.end()));}
            }

        bool lookup_update()
        {
            if (requests_.empty()) {return false;}
            if (cache_storage_.empty()) throw std::runtime_error("Cache storage is empty");

            auto key = std::move(requests_.front());
            requests_.pop_front();

            if (cache_storage_.contains(key)) return true;

            int requests_next_occurrence = occurrence_table_.first(key);

            if (requests_next_occurrence != occurrence_table<key_T>::no_next)
            {
                if (full())
                {
                    auto [iter, next_occurrence] = find_latest();

                    if (next_occurrence != occurrence_table<key_T>::no_next && next_occurrence < requests_next_occurrence)
                    {
                        occurrence_table_.pop_first(key);
                        return false;
                    }
                    else
                    {
                        pages_.erase(iter->second);
                        occurrence_table_.pop_first(iter->first);
                        cache_storage_.erase(iter);
                    }
                }

            pages_.emplace_back(slow_get_page_(key));
            cache_storage_.emplace(key, std::prev(pages_.end()));

            }

            return false;
        }

        bool full() const {return pages_.size() == cache_capacity_; }

    private:

        std::pair<typename std::unordered_map<key_T, iterator>::iterator, int> find_latest()
        {
            int latest_occurrence = 0;
            auto iter = cache_storage_.begin();
            auto latest_iter = iter;

            for (auto end_iter = cache_storage_.end(); iter != end_iter; ++iter)
            {
                int next_occurrence = occurrence_table_.first(iter->first);

                if (next_occurrence == occurrence_table<key_T>::no_next)
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