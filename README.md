# Memecache

## _A minimal, no pain, in-memory caching library for general use_

This is minimal, _**thread-safe caching library**_, with the support for multiple cache eviction policies and can also extend _custom cache eviction policies_.

The currently supported cache eviction policies include:

- _First-In/First-Out (FIFO)_
- _Last-In/First-Out (LIFO)_
- _Least Recently Used (LRU)_

An exhaustive list of cache algorithms can be found here - [Wikipedia](https://en.wikipedia.org/wiki/Cache_algorithms)

## Usage

To use this library, it is necessary to include the header containing the _cache implementation_ (`cache.hpp` file)
and the corresponding appropriate headers containing the required cache eviction policy as per the requirement.
If a policy is not mentioned explicitly, then `NoCachePolicy` is be implemented whereby the replacement candidate for removal is chosen to be the first key that was added in the internal `key_storage` container.

Currently there are three cache eviction policies supported:

- `fifo_cache_policy.hpp`
- `lifo_cache_policy.hpp`
- `lru_cache_policy.hpp`

### An example usage of the LRU policy:

```cpp
#include "cache.hpp"
#include "lru_cache_policy.hpp"

// alias for easy class typing
template <typename Key, typename Value>
using lru_cache_t = typename caches::fixed_sized_cache<Key, Value, caches::LRUCachePolicy>;

void memecache(...) {
  constexpr std::size_t CACHE_SIZE = 256;
  lru_cache_t<char, int> cache(CACHE_SIZE);

  lru_cache.Put('M', 24);
  lru_cache.Put('P', 5);
  lru_cache.Put('B', 2001);

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
  /*
  Output:

    Using LRU Eviction Policy
    Value for key 'M': 24
    Value for key 'P': 5
    Value for key 'B': 2001

  */
}
```

### An example usage of _memecache_ demonstrating its thread-safety:

```cpp
#include "include/cache.hpp"
#include "include/fifo_cache_policy.hpp"
#include "include/lifo_cache_policy.hpp"
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

int main()
{
    std::cout << "Hello Enterpret!\n";

    constexpr std::size_t CACHE_SIZE = 256;
    fifo_cache_t<std::string, int> fifo_cache(CACHE_SIZE);
    lifo_cache_t<std::string, int> lifo_cache(CACHE_SIZE);

    // creating multiple threads to perform cache operations concurrently
    std::thread t1(cache_operations_fifo, std::ref(fifo_cache), "Hello", 100, true);
    std::thread t2(cache_operations_fifo, std::ref(fifo_cache), "world", 6996, true);
    std::thread t3(cache_operations_fifo, std::ref(fifo_cache), "Hello", 0, false);
    std::thread t4(cache_operations_fifo, std::ref(fifo_cache), "world", 0, false);

    std::thread t5(cache_operations_lifo, std::ref(lifo_cache), "Hello", 200, true);
    std::thread t6(cache_operations_lifo, std::ref(lifo_cache), "world", 7997, true);
    std::thread t7(cache_operations_lifo, std::ref(lifo_cache), "Hello", 0, false);
    std::thread t8(cache_operations_lifo, std::ref(lifo_cache), "world", 0, false);



    // joining the threads and waiting for their completion
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    t5.join();
    t6.join();
    t7.join();
    t8.join();

    return 0;
}


/*
Output

Hello Enterpret!
==============================================================================
Using LIFO Eviction Policy
Value for key 'Hello': 200
==============================================================================
Using LIFO Eviction Policy
Value for key 'world': 7997
==============================================================================
Using FIFO Eviction Policy
Value for key 'Hello': 100
==============================================================================
Using FIFO Eviction Policy
Value for key 'world': 6996
==============================================================================
*/

```

A more exhaustive usage and demonstration of the library is shown in the `main.cpp` and `mainthread.cpp` files. Run the `./main`
and `./mainthread` executables after building the project for demonstration purpose.

### Creating _Custom Cache Eviction Policies_

To implement a custom cache eviction or cache replacement policy, include the `cache_policy.hpp` header file containing the _cache policy interface_ and subsequently override the `Insert(...)`, `Touch(...)`, `Erase(...)` and `ReplacementCandidate(...)` methods as per the requirements.

```cpp
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
```

### Requirements

- A compatible C++11 compiler

### Cloning, building and running locally

- Clone the repository

```console
    git clone git@github.com:sanam2405/memecache.git
```

- Install _cmake_

```console
    brew install cmake
```

- Create a `build` directory in the root of the project

```console
    mkdir build
    cd build
```

- Generate the build files

```console
    cmake ..
```

- Build the project

For building, use `make` if available

```console
    make
```

Otherwise use the below command for building

```console
    cmake --build .
```

- Run the executables

```console
    ./main
    ./mainthread
```

## _Built with ❤️ by [Manas](https://sanam.live)_
