#pragma once
#include <vector>
#include <thread>

/**
 * @class JoinThreads
 * @brief A RAII helper class to automatically join a vector of threads on destruction.
 *
 * Ensures that all threads in the provided vector are joined when the object goes out of scope.
 * This helps avoid std::terminate due to unjoined joinable threads.
 */
class JoinThreads
{
public:
    /**
     * @brief Constructs a JoinThreads helper.
     *
     * @param threads A reference to a vector of std::thread objects that need to be joined.
     */
    explicit JoinThreads(std::vector<std::thread>& threads);

    /**
     * @brief Joins all joinable threads in the vector.
     *
     * Destructor ensures proper cleanup by joining all threads.
     */
    ~JoinThreads();

private:
    /**
     * @brief Reference to the vector of threads to be joined.
     */
    std::vector<std::thread>& threads;
};

