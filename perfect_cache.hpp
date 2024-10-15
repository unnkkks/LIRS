#include <unordered_map>
#include <list>
#include <deque>
#include <iterator>

template <typename key_T, typename page_T, typename page_getter>
class perfect_cache
{
    std::size_t cache_capacity;
    std::size_t hits;
    page_getter slow_get_page;

    std::list<key_T> cache;
    std::unordered_map<key_T, page_T> cache_storage;
    std::deque<key_T> requests;

     public:

        bool lookup_update()
            {
                if (cache_storage.empty()) return false;

                auto key = requests.front();
                requests.pop_front();

                if (cache_storage.count(key)) return true;

                auto request_next_occurence = find_next_occurence(key);

                if (request_next_occurence != no_later_occurencies)
                {
                    if (full())
                    {
                        std::pair<std::iterator, int> candidate_to_remove;
                        auto candidate_to_remove = find_latest();

                        if (candidate_to_remove->second != no_later_occurencies && candidate_to_remove->second < request_next_occurence)
                            return false;
                        else
                            cache_storage.erase(candidate_to_remove->first);
                    }

                cache_storage.emplace_back(key, slow_get_page(key));

                }

                return false;
            }

            bool full() const {return cache.size() == cache_capacity; }

    private:

        int no_later_occurencies = -1;

        int find_next_occurence(const key_T& key)
        {
            auto iter = cache_storage.find(key);


            if (iter == cache.storage.end())
                return no_later_occurencies;
            else
                return static_cast<int>(cache_storage.begin() - iter) + 1;
        }

        std::pair<std::iterator, int> find_latest()
        {
            int latest_occurrence = 0;
            auto iter = cache_storage.begin();
            auto latest_iter = iter;

            for (auto end_iter = cache_storage.end(); iter != end_iter; ++iter)
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