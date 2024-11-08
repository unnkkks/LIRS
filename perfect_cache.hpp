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

    std::list<key_T> cache_;
    std::unordered_map<key_T, page_T> cache_storage_;
    std::deque<key_T> requests_;
    using iterator = typename std::vector<requests_>::iterator;

     public:

        template<std::forward_iterator Iterator>
        perfect_cache(size_t capacity, page_getter slow_get_page, Iterator begin, Iterator end):
            {requests_(begin, end), cache_capacity_(capacity), slow_get_page(slow_get_page_)}

        bool lookup_update()
        {
            if (cache_storage_.empty()) throw std::cache_miss{"Cache storage is empty"};

            auto key = std::move(requests.front());
            requests_.pop_front();

            if (cache_storage_.contains(key)) return true;

            auto request_next_occurence = find_next_occurence(key);

            if (request_next_occurence != no_later_occurencies)
            {
                if (full())
                {
                    auto [iterator, next_occurence] = find_latest();

                    if (next_occurence != no_later_occurencies && next_occurence < request_next_occurence)
                        return false;
                    else
                        cache_storage_.erase(iterator);
                }

            cache_storage_.emplace_back(key, slow_get_page_(key));

            }

            return false;
        }

        bool full() const {return cache_.size() == cache_capacity_; }

    private:

        constexpr no_later_occurencies = -1;

        int find_next_occurence(const key_T& key)
        {
            auto iter = requests_.find(key);


            if (iter == requests_.end())
                return no_later_occurencies;
            else
                return std::distance(cache_storage_.begin(), iter) + 1;
        }

        std::pair<std::iterator, int> find_latest()
        {
            int latest_occurrence = 0;
            auto iter = cache_storage_.begin();
            auto latest_iter = iter;

            for (auto end_iter = cache_storage_.end(); iter != end_iter; ++iter)
            {
                int next_occurrence = find_next_occurrence(iter->second);

                if (next_occurrence == no_later_occurencies)
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