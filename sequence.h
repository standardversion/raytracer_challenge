#pragma once
#include <vector>

/**
 * @brief A utility class that provides a sequence of values for sampling or iteration.
 *
 * This is commonly used in rendering contexts (like area lights) to supply a repeating or randomized
 * series of values, such as jitter offsets for anti-aliasing or soft shadows. The sequence wraps
 * around when it reaches the end.
 */
class Sequence
{
public:
    /**
     * @brief Constructs a Sequence with one or more values.
     *
     * This templated constructor accepts any number of arguments convertible to `double` and
     * stores them as the sequence values. The sequence will cycle through these values in the
     * order they are provided.
     *
     * @tparam Args A variadic pack of arguments convertible to `double`.
     * @param args The values to store in the sequence.
     *
     * @code
     * Sequence seq(0.1, 0.3, 0.7);  // Creates a repeating sequence of three values.
     * @endcode
     */
    template<typename ...Args>
    Sequence(Args ...args)
        : values{ static_cast<double>(args)... }
    {
    }

    /**
     * @brief Retrieves the next value in the sequence.
     *
     * This method returns the next value and advances the internal index. If the end of the
     * sequence is reached, it wraps around to the beginning.
     *
     * @return The next value in the sequence as a `double`.
     *
     * @throws std::invalid_argument if the sequence is empty.
     */
    double next();

private:
    std::size_t index{ 0 };          ///< Current index in the sequence.
    std::vector<double> values;      ///< Container for the sequence of values.
};
