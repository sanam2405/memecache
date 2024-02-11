#include <iostream>
#include <thread>
#include "include/fifo_cache_policy.hpp"
#include "include/cache.hpp"

// alias for easy class typing
template <typename Key, typename Value>
using fifo_cache_t = typename caches::fixed_sized_cache<Key, Value, caches::FIFOCachePolicy>;

void cache_operations(fifo_cache_t<std::string, int> &cache, const std::string &key, int value, bool insert)
{
    if (insert)
    {
        cache.Put(key, value);
    }
    else
    {
        try
        {
            std::cout << "Value for key '" << key << "': " << cache.Get(key) << '\n';
        }
        catch (const std::range_error &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

int main()
{
    std::cout << "Hello Enterpret!\n";

    constexpr std::size_t CACHE_SIZE = 256;
    fifo_cache_t<std::string, int> cache(CACHE_SIZE);

    // creating multiple threads to perform cache operations concurrently
    std::thread t1(cache_operations, std::ref(cache), "Hello", 100, true);
    std::thread t2(cache_operations, std::ref(cache), "world", 6996, true);
    std::thread t3(cache_operations, std::ref(cache), "Hello", 0, false);
    std::thread t4(cache_operations, std::ref(cache), "world", 0, false);

    // joining the threads and waiting for their completion
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
