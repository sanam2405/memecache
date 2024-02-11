#include <iostream>
#include "include/fifo_cache_policy.hpp"
#include "include/cache.hpp"

// alias for easy class typing
template <typename Key, typename Value>
using fifo_cache_t = typename caches::fixed_sized_cache<Key, Value, caches::FIFOCachePolicy>;

int main()
{

    std::cout << "Hello Enterpret!\n";

    constexpr std::size_t CACHE_SIZE = 256;
    fifo_cache_t<std::string, int> cache(CACHE_SIZE);

    cache.Put("Hello", 100);
    cache.Put("world", 6996);

    std::cout << "Value for key '"
              << "Hello"
              << "': " << cache.Get("Hello") << '\n';
    std::cout << "Value for key '"
              << "world"
              << "': " << cache.Get("world") << '\n';

    return 0;
}