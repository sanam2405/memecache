#include "include/cache.hpp"
#include "include/fifo_cache_policy.hpp"
#include "include/lifo_cache_policy.hpp"
#include "include/lru_cache_policy.hpp"
#include <iostream>
#include <mutex>
#include <thread>

// mutex to synchronize access to std::cout
std::mutex cout_mutex;

// alias for easy class typing
template <typename Key, typename Value>
using fifo_cache_t = caches::fixed_sized_cache<Key, Value, caches::FIFOCachePolicy>;
template <typename Key, typename Value>
using lifo_cache_t = caches::fixed_sized_cache<Key, Value, caches::LIFOCachePolicy>;
template <typename Key, typename Value>
using lru_cache_t = caches::fixed_sized_cache<Key, Value, caches::LRUCachePolicy>;

void printLine() { std::cout << "==============================================================================\n"; }

void cache_operations_fifo(fifo_cache_t<std::string, int>& cache, const std::string& key, int value, bool insert)
{
    if (insert)
    {
        cache.Put(key, value);
    }
    else
    {
        try
        {
            // lock cout access
            std::lock_guard<std::mutex> lock(cout_mutex);
            printLine();
            std::cout << "Using FIFO Eviction Policy\n"
                      << "Value for key '" << key << "': " << cache.Get(key) << '\n';
        }
        catch (const std::range_error& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

void cache_operations_lifo(lifo_cache_t<std::string, int>& cache, const std::string& key, int value, bool insert)
{
    if (insert)
    {
        cache.Put(key, value);
    }
    else
    {
        try
        {
            // lock cout access
            std::lock_guard<std::mutex> lock(cout_mutex);
            printLine();
            std::cout << "Using LIFO Eviction Policy\n"
                      << "Value for key '" << key << "': " << cache.Get(key) << '\n';
        }
        catch (const std::range_error& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

void cache_operations_lru(lru_cache_t<std::string, int>& cache, const std::string& key, int value, bool insert)
{
    if (insert)
    {
        cache.Put(key, value);
    }
    else
    {
        try
        {
            // lock cout access
            std::lock_guard<std::mutex> lock(cout_mutex);
            printLine();
            std::cout << "Using LRU Eviction Policy\n"
                      << "Value for key '" << key << "': " << cache.Get(key) << '\n';
        }
        catch (const std::range_error& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

int main()
{
    std::cout << "Hello Enterpret!\n";

    constexpr std::size_t CACHE_SIZE = 256;
    fifo_cache_t<std::string, int> fifo_cache(CACHE_SIZE);
    lifo_cache_t<std::string, int> lifo_cache(CACHE_SIZE);
    lru_cache_t<std::string, int> lru_cache(CACHE_SIZE);

    // creating multiple threads to perform cache operations concurrently
    std::thread t1(cache_operations_fifo, std::ref(fifo_cache), "Hello", 100, true);
    std::thread t2(cache_operations_fifo, std::ref(fifo_cache), "world", 6996, true);
    std::thread t3(cache_operations_fifo, std::ref(fifo_cache), "Hello", 0, false);
    std::thread t4(cache_operations_fifo, std::ref(fifo_cache), "world", 0, false);

    std::thread t5(cache_operations_lifo, std::ref(lifo_cache), "Hello", 200, true);
    std::thread t6(cache_operations_lifo, std::ref(lifo_cache), "world", 7997, true);
    std::thread t7(cache_operations_lifo, std::ref(lifo_cache), "Hello", 0, false);
    std::thread t8(cache_operations_lifo, std::ref(lifo_cache), "world", 0, false);

    std::thread t9(cache_operations_lru, std::ref(lru_cache), "Hello", 300, true);
    std::thread t10(cache_operations_lru, std::ref(lru_cache), "world", 8998, true);
    std::thread t11(cache_operations_lru, std::ref(lru_cache), "Hello", 0, false);
    std::thread t12(cache_operations_lru, std::ref(lru_cache), "world", 0, false);


    // joining the threads and waiting for their completion
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    t5.join();
    t6.join();
    t7.join();
    t8.join();

    t9.join();
    t10.join();
    t11.join();
    t12.join();

    return 0;
}
