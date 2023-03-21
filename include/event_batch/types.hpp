/**
 * @file
 * @brief Common types used by the library.
 */

#ifndef EVENT_BATCH_TYPES_HPP
#define EVENT_BATCH_TYPES_HPP

#include <cstdint>
#include <vector>

namespace event_batch
{
/**
 * @brief Alias type for <a
 * href="https://en.cppreference.com/w/cpp/container/vector">std::vector</a>.
 */
template <typename T, typename Allocator = std::allocator<T>>
using StdVector = typename std::vector<T, Allocator>;

/**
 * @brief Structure representing an event.
 *
 * This structure supports events with an arbitrary number of polarities.
 */
struct __attribute__((__packed__)) Event
{
  /**
   * @brief Timestamp of the event.
   */
  uint64_t t;
  /**
   * @brief Horizontal coordinate of the event.
   */
  uint16_t x;
  /**
   * @brief Vertical coordinate of the event.
   */
  uint16_t y;
  /**
   * @brief Polarity of the event.
   */
  uint16_t p;
};

/**
 * @brief Event decay structure.
 */
struct Decay
{
  /**
   * @brief Previous timestamp \f$[\text{microseconds}]\f$.
   */
  uint64_t t;
  /**
   * @brief Event decay in \f$[0,1]\f$.
   */
  float decay;
  /**
   * @brief Auxiliary variable that counts the incoming number of events.
   */
  float n_decay;
  /**
   * @brief Auxiliary variable that estimates the event time decay
   * \f$[\text{microseconds}]\f$.
   */
  float t_decay;
  /**
   * @brief Estimated event rate \f$[\text{events}/\text{microseconds}]\f$.
   */
  float rate;

  /**
   * @brief Resets the context.
   *
   * @param t_decay_first Initial time rate assumption to bootstrap the rate
   * estimator \f$[\text{microseconds}]\f$.
   */
  void
  reset(const uint64_t t_decay_first)
  {
    t = 0;
    decay = 1;
    n_decay = 0;
    t_decay = t_decay_first;
    rate = 0;
  }
};
}  // namespace event_batch

#endif  // EVENT_BATCH_TYPES_HPP
