#include "include/cache.hpp"
#include "include/fifo_cache_policy.hpp"
#include "include/lifo_cache_policy.hpp"
#include <iostream>

// alias for easy class typing
template <typename Key, typename Value>
using fifo_cache_t = typename caches::fixed_sized_cache<Key, Value, caches::FIFOCachePolicy>;
template <typename Key, typename Value>
using lifo_cache_t = typename caches::fixed_sized_cache<Key, Value, caches::LIFOCachePolicy>;

int main()
{

    std::cout << "Hello Enterpret!\n";

    constexpr std::size_t CACHE_SIZE = 256;
    fifo_cache_t<std::string, int> fifo_cache(CACHE_SIZE);
    lifo_cache_t<std::string, int> lifo_cache(CACHE_SIZE);

    fifo_cache.Put("Hello", 80);
    fifo_cache.Put("enterpret", 81);

    lifo_cache.Put("Backend", 40);
    lifo_cache.Put("intern", 4);

    std::cout << "Using FIFO Eviction Policy\n"
              << "Value for key '"
              << "Hello"
              << "': " << fifo_cache.Get("Hello") << '\n';
    std::cout << "Value for key '"
              << "enterpret"
              << "': " << fifo_cache.Get("enterpret") << '\n';

    std::cout << "Using LIFO Eviction Policy\n"
              << "Value for key '"
              << "Backend"
              << "': " << lifo_cache.Get("Backend") << '\n';
    std::cout << "Value for key '"
              << "intern"
              << "': " << lifo_cache.Get("intern") << '\n';

    return 0;
}
