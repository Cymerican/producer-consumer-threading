# Multithreaded Prime Generator — CSE 4733 Operating Systems

A multithreaded C++ program that spawns N concurrent threads, each independently computing the largest prime number it can find within a 30-second time limit. Demonstrates core concurrency concepts including thread management, mutex synchronization, thread-local storage, and voluntary context switching.

---

## Features

- **Configurable thread count** — specify number of threads via command-line argument
- **Thread-local storage** — each thread maintains its own independent `number` and `prime` variables
- **Mutex-protected output** — `std::mutex` with `std::lock_guard` prevents jumbled console output
- **Voluntary context switching** — `std::this_thread::yield()` allows cooperative multitasking
- **30-second time limit** — each thread runs for exactly 30 seconds using `std::chrono`
- **Prime computation** — each thread independently finds the largest prime it can in its time window
- **Boost program options** — clean CLI argument parsing with error handling

---

## How It Works

1. The program reads the number of threads from the command line
2. Each thread starts independently, resetting its own thread-local counter to 2
3. Threads run concurrently, each checking numbers for primality
4. `std::this_thread::yield()` is called each iteration to allow other threads to run
5. After 30 seconds, each thread locks the shared mutex and prints its result
6. The main thread waits for all threads to finish with `join()`

---

## How to Build and Run

### Prerequisites
- GCC / G++
- CMake 3.10+
- Boost libraries (`libboost-program-options-dev`)

### Build
```bash
mkdir build
cd build
cmake ..
make
```

### Run
```bash
./Lab9 --threads 4
```

Replace `4` with any number of threads you want to spawn.

### Example Output
```
Thread 2 ended. Total run time: 30 seconds. Largest prime calculated: 982451653
Thread 0 ended. Total run time: 30 seconds. Largest prime calculated: 982451653
Thread 1 ended. Total run time: 30 seconds. Largest prime calculated: 982451653
Thread 3 ended. Total run time: 30 seconds. Largest prime calculated: 982451653
```

---

## Key Concepts Demonstrated

- **POSIX-style threading** with `std::thread`
- **Mutex synchronization** — preventing race conditions on shared output
- **Thread-local storage** — `thread_local` keyword for per-thread variables
- **Voluntary context switching** — cooperative scheduling with `yield()`
- **Timing with std::chrono** — precise time measurement for thread control
- **CLI argument parsing** with Boost program options

---

## Built With

- C++17
- CMake
- Boost (program_options)

---

## Author

**Cyrus** — Computer Science, Mississippi State University  
[GitHub](https://github.com/Cymerican)
