// LIFO cache policy implementation
#ifndef LIFO_CACHE_POLICY_HPP
#define LIFO_CACHE_POLICY_HPP

#include "cache_policy.hpp"
#include <list>
#include <unordered_map>

namespace caches
{
    /**
     * LIFO (Last in, first out) cache policy
     * This policy removes the last added element when the cache is full.
     * For example, if keys are added in the order A -> B -> C, and the cache is at capacity,
     * the LIFO policy will choose C as the replacement candidate.
     * Subsequent replacements will follow the reverse order of addition, such as B, A, and so on.
     * Key - The type of key this policy works with
     */
    template <typename Key> class LIFOCachePolicy : public ICachePolicy<Key>
    {
    public:
        using lifo_iterator = typename std::list<Key>::const_iterator;

        LIFOCachePolicy() = default;
        ~LIFOCachePolicy() = default;

        // handles element insertion in the cache
        void Insert(const Key& key) override
        {
            lifo_stack.emplace_front(key);
            key_lookup[key] = lifo_stack.begin();
        }

        // handles request to the key-value in the cache
        void Touch(const Key& key) noexcept override
        {
            // does not do anything in the LIFO strategy
            (void)key;
        }

        // handles element deletion from the cache
        void Erase(const Key& key) noexcept override
        {
            auto element = key_lookup[key];
            lifo_stack.erase(element);
            key_lookup.erase(key);
        }

        // returns the key of the replacement candidate for the FIFO policy
        const Key& ReplacementCandidate() const noexcept override { return lifo_stack.front(); }

    private:
        std::list<Key> lifo_stack;
        std::unordered_map<Key, lifo_iterator> key_lookup;
    };
} // namespace caches

#endif // LIFO_CACHE_POLICY_HPP