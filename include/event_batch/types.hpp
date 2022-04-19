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
}  // namespace event_batch

#endif  // EVENT_BATCH_TYPES_HPP
