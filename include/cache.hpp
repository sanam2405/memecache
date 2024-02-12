// A generalised cache implementation
#ifndef CACHE_HPP
#define CACHE_HPP

#include "cache_policy.hpp"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <limits>
#include <memory>
#include <mutex>
#include <unordered_map>

namespace caches
{

    /*
     * Fixed sized cache that can be used with different eviction policies
     * Key - Type of a key [the key should be a hash-able one]
     * Value - Type of a value stored in the cache
     * Policy - Type of a policy to be used with the cache
     * HashMap - Type of the hashmap to use for caching. Might be extended to have `std::unordered_map` compatible
     * interfaces
     */
    template <typename Key, typename Value, template <typename> class Policy = NoCachePolicy,
              typename HashMap = std::unordered_map<Key, Value>>
    class fixed_sized_cache
    {
    public:
        using iterator = typename HashMap::iterator;
        using const_iterator = typename HashMap::const_iterator;
        using operation_guard = typename std::lock_guard<std::mutex>;
        using on_erase_cb = typename std::function<void(const Key& key, const Value& value)>;

        /*
         * Fixed sized cache constructor
         * throws std::invalid_argument if max_cache_size == 0
         * max_size -  Maximum size of the cache
         * policy - Cache policy to use
         * on_erase on_erase_cb - Callback function called when cache's element get erased
         */
        explicit fixed_sized_cache(
            size_t max_size, const Policy<Key> policy = Policy<Key>{},
            on_erase_cb on_erase = [](const Key&, const Value&) {})
                : cache_policy{policy}, max_cache_size{max_size}, on_erase_callback{on_erase}
        {
            if (max_cache_size == 0)
            {
                throw std::invalid_argument{"Size of the cache should be non-zero"};
            }
        }

        ~fixed_sized_cache() noexcept { Clear(); }

        /*
         * Puts element into the cache
         * key - The Key to which value has to be assigned
         * value - The Value to assign to the given key
         */
        void Put(const Key& key, const Value& value) noexcept
        {
            operation_guard lock{safe_operation};
            auto element_iterator = FindElem(key);

            if (element_iterator == cache_items_map.end())
            {
                // adds new element to the cache
                if (cache_items_map.size() + 1 > max_cache_size)
                {
                    auto displacement_candidate_key = cache_policy.ReplacementCandidate();

                    Erase(displacement_candidate_key);
                }

                Insert(key, value);
            }
            else
            {
                // updates previous value
                Update(key, value);
            }
        }

        /*
         * Tries to get an element by the given key from the cache
         * key - Tries to get the element by key
         * Returns a pair of iterator that points to the element and a boolean value that shows
         * if the get operation has been successful or not. If pair's boolean value is true,
         * the returned iterator can be used to access the element. Otherwise, if pair's boolean value
         * is false, the element is not presented in the cache.
         */
        std::pair<const_iterator, bool> TryGet(const Key& key) const noexcept
        {
            operation_guard lock{safe_operation};
            return GetInternal(key);
        }

        /*
         * Gets the element from the cache if the element is present
         * key - Element's key that we are trying to get
         * Returns reference to the value stored by the specified key in the cache
         */
        const Value& Get(const Key& key) const
        {
            operation_guard lock{safe_operation};
            auto element = GetInternal(key);

            if (element.second)
            {
                return element.first->second;
            }
            else
            {
                throw std::range_error{"No such element in the cache"};
            }
        }

        /*
         * Checks if the given key is presented in the cache
         * key - Element's key that is to be checked
         * Returns true if the element key is presented
         * Returns false if the element key is not presented
         */
        bool Cached(const Key& key) const noexcept
        {
            operation_guard lock{safe_operation};
            return FindElem(key) != cache_items_map.cend();
        }

        /*
         * Returns the number of elements currently present in the cache
         */
        std::size_t Size() const
        {
            operation_guard lock{safe_operation};

            return cache_items_map.size();
        }

        /*
         * Removes an element specified by key
         * key - Key of the element that is to be removed
         * Returns true if the element specified by the key was found and successfully deleted
         * Returns false if the element is not present in a cache and could not be found
         */
        bool Remove(const Key& key)
        {
            operation_guard lock{safe_operation};

            auto element = FindElem(key);

            if (element == cache_items_map.end())
            {
                return false;
            }

            Erase(element);

            return true;
        }

    protected:
        void Clear()
        {
            operation_guard lock{safe_operation};

            std::for_each(begin(), end(),
                          [&](const std::pair<const Key, Value>& element) { cache_policy.Erase(element.first); });
            cache_items_map.clear();
        }

        const_iterator begin() const noexcept { return cache_items_map.cbegin(); }

        const_iterator end() const noexcept { return cache_items_map.cend(); }

    protected:
        void Insert(const Key& key, const Value& value)
        {
            cache_policy.Insert(key);
            cache_items_map.emplace(std::make_pair(key, value));
        }

        void Erase(const_iterator element)
        {
            cache_policy.Erase(element->first);
            on_erase_callback(element->first, element->second);
            cache_items_map.erase(element);
        }

        void Erase(const Key& key)
        {
            auto element_iterator = FindElem(key);

            Erase(element_iterator);
        }

        void Update(const Key& key, const Value& value)
        {
            cache_policy.Touch(key);
            cache_items_map[key] = value;
        }

        const_iterator FindElem(const Key& key) const { return cache_items_map.find(key); }

        std::pair<const_iterator, bool> GetInternal(const Key& key) const noexcept
        {
            auto element_iterator = FindElem(key);

            if (element_iterator != end())
            {
                cache_policy.Touch(key);
                return {element_iterator, true};
            }

            return {element_iterator, false};
        }

    private:
        HashMap cache_items_map;
        mutable Policy<Key> cache_policy;
        mutable std::mutex safe_operation;
        std::size_t max_cache_size;
        on_erase_cb on_erase_callback;
    };
} // namespace caches

#endif // CACHE_HPP