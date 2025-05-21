#pragma once
#include <mutex>
#include <queue>

/**
 * @brief A thread-safe queue implementation using mutex and condition variable.
 *
 * This class provides safe concurrent access for pushing and popping elements
 * in a multithreaded environment. It supports both blocking (`wait_and_pop`) and
 * non-blocking (`try_pop`) operations.
 *
 * @tparam T The type of elements stored in the queue.
 */
template<typename T>
class ThreadsafeQueue
{
public:
    /**
     * @brief Constructs an empty thread-safe queue.
     */
    ThreadsafeQueue() {};

    /**
     * @brief Pushes a new value into the queue.
     *
     * @param new_value The value to be added. It will be moved into the queue.
     */
    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk{ mut };
        data_queue.push(std::move(new_value));
        data_cond.notify_one();
    }

    /**
     * @brief Blocks until an item is available, then pops it into the provided reference.
     *
     * @param value Reference to store the popped item.
     */
    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lk{ mut };
        data_cond.wait(lk, [this] { return !data_queue.empty(); });
        value = std::move(data_queue.front());
        data_queue.pop();
    }

    /**
     * @brief Blocks until an item is available, then returns it as a shared pointer.
     *
     * @return A shared pointer to the popped item.
     */
    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lk{ mut };
        data_cond.wait(lk, [this] { return !data_queue.empty(); });
        std::shared_ptr<T> res{ std::make_shared<T>(std::move(data_queue.front())) };
        data_queue.pop();
        return res;
    }

    /**
     * @brief Tries to pop an item without blocking.
     *
     * @param value Reference to store the item if available.
     * @return true if an item was popped, false if the queue was empty.
     */
    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lk{ mut };
        if (data_queue.empty()) return false;
        value = std::move(data_queue.front());
        data_queue.pop();
        return true;
    }

    /**
     * @brief Tries to pop an item without blocking, returning a shared pointer.
     *
     * @return A shared pointer to the popped item, or nullptr if the queue was empty.
     */
    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lk{ mut };
        if (data_queue.empty()) return std::shared_ptr<T>();
        std::shared_ptr<T> res{ std::make_shared<T>(std::move(data_queue.front())) };
        data_queue.pop();
        return res;
    }

    /**
     * @brief Checks whether the queue is empty.
     *
     * @return true if the queue is empty, false otherwise.
     */
    bool empty() const
    {
        std::lock_guard<std::mutex> lk{ mut };
        return data_queue.empty();
    }

private:
    /**
     * @brief Mutex to protect access to the internal queue.
     */
    mutable std::mutex mut;

    /**
     * @brief The underlying standard queue used for storage.
     */
    std::queue<T> data_queue;

    /**
     * @brief Condition variable used to signal availability of data.
     */
    std::condition_variable data_cond;
};
