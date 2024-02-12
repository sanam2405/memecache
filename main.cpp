#include "include/cache.hpp"
#include "include/fifo_cache_policy.hpp"
#include "include/lifo_cache_policy.hpp"
#include "include/lru_cache_policy.hpp"
#include <iostream>

// alias for easy class typing
template <typename Key, typename Value>
using fifo_cache_t = typename caches::fixed_sized_cache<Key, Value, caches::FIFOCachePolicy>;
template <typename Key, typename Value>
using lifo_cache_t = typename caches::fixed_sized_cache<Key, Value, caches::LIFOCachePolicy>;
template <typename Key, typename Value>
using lru_cache_t = typename caches::fixed_sized_cache<Key, Value, caches::LRUCachePolicy>;

void printLine() { std::cout << "==============================================================================\n"; }

int main()
{

    std::cout << "Hello Enterpret!\n";

    constexpr std::size_t CACHE_SIZE = 256;
    fifo_cache_t<int, int> fifo_cache(CACHE_SIZE);
    lifo_cache_t<std::string, int> lifo_cache(CACHE_SIZE);
    lru_cache_t<char, int> lru_cache(CACHE_SIZE);

    fifo_cache.Put(9, 81);
    fifo_cache.Put(5, 25);

    lifo_cache.Put("Backend", 40);
    lifo_cache.Put("intern", 4);

    lru_cache.Put('M', 24);
    lru_cache.Put('P', 5);
    lru_cache.Put('B', 2001);

    printLine();

    std::cout << "Using FIFO Eviction Policy\n"
              << "Value for key '"
              << "9"
              << "': " << fifo_cache.Get(9) << '\n';
    std::cout << "Value for key '"
              << "5"
              << "': " << fifo_cache.Get(5) << '\n';

    printLine();

    std::cout << "Using LIFO Eviction Policy\n"
              << "Value for key '"
              << "Backend"
              << "': " << lifo_cache.Get("Backend") << '\n';
    std::cout << "Value for key '"
              << "intern"
              << "': " << lifo_cache.Get("intern") << '\n';

    printLine();

    std::cout << "Using LRU Eviction Policy\n"
              << "Value for key '"
              << "M"
              << "': " << lru_cache.Get('M') << '\n';
    std::cout << "Value for key '"
              << "P"
              << "': " << lru_cache.Get('P') << '\n';
    std::cout << "Value for key '"
              << "B"
              << "': " << lru_cache.Get('B') << '\n';

    printLine();

    return 0;
}
