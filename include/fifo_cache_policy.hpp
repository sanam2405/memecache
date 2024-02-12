// FIFO cache policy implementation
#ifndef FIFO_CACHE_POLICY_HPP
#define FIFO_CACHE_POLICY_HPP

#include "cache_policy.hpp"
#include <list>
#include <unordered_map>

namespace caches
{
    /**
     * FIFO (First in, first out) cache policy
     * This policy removes the first added element when the cache is full.
     * For example, if keys are added in the order A -> B -> C, and the cache is at capacity,
     * the FIFO policy will choose A as the replacement candidate.
     * Subsequent replacements will follow the order of addition, such as B, C, and so on.
     * Key - The type of key this policy works with
     */
    template <typename Key> class FIFOCachePolicy : public ICachePolicy<Key>
    {
    public:
        using fifo_iterator = typename std::list<Key>::const_iterator;

        FIFOCachePolicy() = default;
        ~FIFOCachePolicy() = default;

        // handles element insertion in the cache
        void Insert(const Key& key) override
        {
            fifo_queue.emplace_front(key);
            key_lookup[key] = fifo_queue.begin();
        }

        // handles request to the key-value in the cache
        void Touch(const Key& key) noexcept override
        {
            // does not do anything in the FIFO strategy
            (void)key;
        }

        // handles element deletion from the cache
        void Erase(const Key& key) noexcept override
        {
            auto element = key_lookup[key];
            fifo_queue.erase(element);
            key_lookup.erase(key);
        }

        // returns the key of the replacement candidate for the FIFO policy
        const Key& ReplacementCandidate() const noexcept override { return fifo_queue.back(); }

    private:
        std::list<Key> fifo_queue;
        std::unordered_map<Key, fifo_iterator> key_lookup;
    };
} // namespace caches

#endif // FIFO_CACHE_POLICY_HPP
