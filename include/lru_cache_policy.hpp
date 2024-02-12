// LRU cache policy implementation
#ifndef LRU_CACHE_POLICY_HPP
#define LRU_CACHE_POLICY_HPP

#include "cache_policy.hpp"
#include <list>
#include <unordered_map>

namespace caches
{
    /*
     * LRU (Least Recently Used) cache policy
     * This policy removes the least recently used element.
     * Essentially, when the cache needs to replace an element due to being full,
     * it picks the one that hasn't been used for the longest time. For instance,
     * suppose a cache with a maximum size of 3 and 3 elements are added in order: A -> B -> C
     * Then, if A is accessed and then B is accessed, C becomes the least recently used.
     * If another element D is added, C becomes the replacement candidate since it was accessed the earliest.
     * Cache elements: A -> B -> C
     * LRU element in the cache: C, B, A
     * Cache access: A touched
     * LRU element in the cache: A, C, B
     * Cache access: B touched
     * LRU element in the cache: B, A, C
     * Put new element: D
     * LRU element in the cache: D, B, A and C is erased/popped
     * LRU replacement candidate: C
     * Key - The type of key this policy works with
     */
    template <typename Key> class LRUCachePolicy : public ICachePolicy<Key>
    {
    public:
        using lru_iterator = typename std::list<Key>::iterator;

        LRUCachePolicy() = default;
        ~LRUCachePolicy() = default;

        void Insert(const Key& key) override
        {
            lru_queue.emplace_front(key);
            key_finder[key] = lru_queue.begin();
        }

        void Touch(const Key& key) override
        {
            // moves the touched element to the beginning of the lru_queue
            lru_queue.splice(lru_queue.begin(), lru_queue, key_finder[key]);
        }

        void Erase(const Key&) noexcept override
        {
            // removes the least recently used element
            key_finder.erase(lru_queue.back());
            lru_queue.pop_back();
        }

        // returns the key of the displacement candidate
        const Key& ReplacementCandidate() const noexcept override { return lru_queue.back(); }

    private:
        std::list<Key> lru_queue;
        std::unordered_map<Key, lru_iterator> key_finder;
    };
} // namespace caches

#endif // LRU_CACHE_POLICY_HPP
