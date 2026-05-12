#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <boost/program_options.hpp>

// Thread local storage for the current number being checked and the largest prime found
thread_local int number = 2;
thread_local int prime = 2;

bool isPrime(int number)
{
    if (number < 2)
        return false;
    for (int i = 2; i * i <= number; i++)
    {
        if (number % i == 0)
        {
            return false;
        }
    }
    return true;
}

void work(int id, std::mutex &cout_mutex)
{
    // Reset thread-local storage for this thread
    number = 2;
    prime = 2;

    // Set time limit to 30 seconds and record start time
    auto time_limit = std::chrono::seconds(30);
    auto start_time = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::seconds(0);

    // Keep checking numbers until 30 seconds have passed
    while (elapsed < time_limit)
    {
        // If the current number is prime, update our thread-local prime
        if (isPrime(number))
        {
            prime = number;
        }

        // Move to the next number
        number++;

        // Yield to let other threads run (voluntary context switch)
        std::this_thread::yield();

        // Update elapsed time
        elapsed = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::high_resolution_clock::now() - start_time);
    }

    // Lock the mutex so output doesn't get mixed between threads
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "Thread " << id << " ended. Total run time: "
              << elapsed.count() << " seconds. Largest prime calculated: "
              << prime << "\n";
}

int main(int argc, char *argv[])
{
    try
    {

        boost::program_options::options_description desc("Allowed options");
        desc.add_options()("help", "produce help message")("threads", boost::program_options::value<int>()->required(), "set the number of threads");

        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);

        if (vm.count("help"))
        {
            std::cout << desc << "\n";
            return 1;
        }

        // Get the number of threads from the command line
        int num_threads = vm["threads"].as<int>();

        // Vector to hold all our thread objects
        std::vector<std::thread> threads;

        // Mutex to make sure cout output doesn't get jumbled
        std::mutex cout_mutex;

        // Spawn each thread, passing its ID and the shared mutex
        for (int i = 0; i < num_threads; i++)
        {
            threads.emplace_back(work, i, std::ref(cout_mutex));
        }

        // Wait for all threads to finish before exiting
        for (auto &t : threads)
        {
            t.join();
        }
    }
    catch (std::system_error &e)
    {
        std::cerr << "System error: " << e.what() << "\n";
        return 1;
    }
    catch (boost::program_options::invalid_option_value &e)
    {
        std::cerr << "Invalid option value: " << e.what() << "\n";
        return 1;
    }
    catch (boost::program_options::unknown_option &e)
    {
        std::cerr << "Unknown option: " << e.what() << "\n";
        return 1;
    }
    catch (boost::program_options::multiple_occurrences &e)
    {
        std::cerr << "Multiple occurrences of the same option: " << e.what() << "\n";
        return 1;
    }
    catch (std::exception &e)
    {
        std::cerr << "Standard exception: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
