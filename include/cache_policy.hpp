// Cache policy interface declaration

#ifndef CACHE_POLICY_HPP
#define CACHE_POLICY_HPP

#include <unordered_set>

namespace caches
{

    /*
     * Cache policy abstract base class
     * Key - Type of a key a policy works with
     */
    template <typename Key> class ICachePolicy
    {
    public:
        virtual ~ICachePolicy() = default;

        /*
         * Handles element insertion in the cache
         * key - Key that should be used by the policy
         */
        virtual void Insert(const Key& key) = 0;

        /*
         * Handles request to the key-value in the cache
         * key - Key that should be used by the policy
         */
        virtual void Touch(const Key& key) = 0;

        /*
         * Handles deletion of key-value from the cache
         * key - Key that should be used by the policy
         */

        virtual void Erase(const Key& key) = 0;

        /*
         * Returns the key of the replacement candidate
         * Key - Replacement candidate's key according to selected eviction policy
         */
        virtual const Key& ReplacementCandidate() const = 0;
    };

    /*
     * This class doesn't perform any caching strategy.
     * It simply stores keys that are inserted into it.
     * When a key needs to be removed, it can remove any key without following specific rules.
     * The replacement candidate for removal is chosen to be the first key that was added.
     * However, since an unordered container is used internally, there's no guarantee that
     * the first or last added key will be removed first.
     */
    template <typename Key> class NoCachePolicy : public ICachePolicy<Key>
    {
    public:
        NoCachePolicy() = default;
        ~NoCachePolicy() noexcept override = default;

        void Insert(const Key& key) override { key_storage.emplace(key); }

        void Touch(const Key& key) noexcept override
        {
            // does not do anything
            (void)key;
        }

        void Erase(const Key& key) noexcept override { key_storage.erase(key); }

        // returns a key of a replacement candidate, by default the beginning candidate
        const Key& ReplacementCandidate() const noexcept override { return *key_storage.cbegin(); }

    private:
        std::unordered_set<Key> key_storage;
    };
} // namespace caches

#endif // CACHE_POLICY_HPP
