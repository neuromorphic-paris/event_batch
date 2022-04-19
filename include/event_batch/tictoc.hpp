/**
 * @file
 * @brief Clock utility to estimate runtime.
 */

#ifndef EVENT_BATCH_TICTOC_HPP
#define EVENT_BATCH_TICTOC_HPP

#include <chrono>
#include <type_traits>

namespace event_batch
{
/**
 * @brief Clock utility to estimate runtime.
 *
 * This class estimates the runtime execution of a piece of code.
 * First, call \ref tic to store the time at which the piece of code starts.
 * Then, call \ref toc after the piece of code to get the execution time.
 */
class TicToc
{
 public:
  /**
   * @brief Alias for time.
   */
  typedef std::chrono::high_resolution_clock::time_point Time;
  /**
   * @brief Alias to report runtime in nanoseconds.
   */
  typedef std::chrono::nanoseconds NanoSeconds;
  /**
   * @brief Alias to report runtime in microseconds.
   */
  typedef std::chrono::microseconds MicroSeconds;
  /**
   * @brief Alias to report runtime in milliseconds.
   */
  typedef std::chrono::milliseconds MilliSeconds;
  /**
   * @brief Alias to report runtime in seconds.
   */
  typedef std::chrono::seconds Seconds;

  /**
   * @brief Stores the starting time.
   */
  inline void
  tic()
  {
    t_ = std::chrono::high_resolution_clock::now();
  }

  /**
   * @brief Estimates the execution time based on the previous \ref tic call.
   *
   * \tparam OrderOfMagnitude Template argument of the runtime magnitude order.
   * The following template arguments are accepted:
   *  - \ref NanoSeconds
   *  - \ref MicroSeconds
   *  - \ref MilliSeconds
   *  - \ref Seconds
   *
   * @return The time difference based on the previous \ref tic call.
   */
  template <typename OrderOfMagnitude>
  inline double
  toc()
  {
    const Time t = std::chrono::high_resolution_clock::now();
    const double t_diff =
        std::chrono::duration_cast<std::chrono::nanoseconds>(t - t_).count();
    if constexpr (std::is_same<OrderOfMagnitude, NanoSeconds>::value)
    {
      return t_diff;
    }
    if constexpr (std::is_same<OrderOfMagnitude, MicroSeconds>::value)
    {
      return t_diff * 1.0e-3;
    }
    if constexpr (std::is_same<OrderOfMagnitude, MilliSeconds>::value)
    {
      return t_diff * 1.0e-6;
    }
    if constexpr (std::is_same<OrderOfMagnitude, Seconds>::value)
    {
      return t_diff * 1.0e-9;
    }
    return -1.0;
  }

 protected:
  /**
   * @brief Stores the starting time.
   */
  Time t_;
};
}  // namespace event_batch

#endif  // EVENT_BATCH_TICTOC_HPP
