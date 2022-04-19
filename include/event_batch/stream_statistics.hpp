/**
 * @file
 * @brief Utility to compute basic statistics of an event stream.
 */

#ifndef EVENT_BATCH_STREAM_STATISTICS_HPP
#define EVENT_BATCH_STREAM_STATISTICS_HPP

#include <cstdint>
#include <iostream>
#include <string>

#include "event_batch/event_stream_statistics.hpp"
#include "event_batch/types.hpp"
#include "sepia.hpp"

namespace event_batch
{
/**
 * @brief Basic event stream statistics storage.
 *
 * This structure stores basic statistics of an event stream.
 * It is provided as a convenience.
 */
struct __attribute__((__packed__)) StreamStatistics
{
  /**
   * @brief Current timestamp \f$[\text{microseconds}]\f$.
   */
  uint64_t t;
  /**
   * @brief First timestamp \f$[\text{microseconds}]\f$.
   */
  uint64_t t_first;
  /**
   * @brief Number of events of the stream.
   */
  uint64_t number_events;
  /**
   * @brief Duration of the event stream \f$[\text{microseconds}]\f$.
   */
  uint64_t duration;
};

/**
 * @brief Computes basic event stream statistics from a file.
 *
 * @tparam Type Type that associates an event stream type name with its byte.
 * Refer to <a href="https://github.com/neuromorphic-paris/sepia">sepia</a> for
 * more information regarding the allowed template arguments.
 * @tparam Event Type of event.
 *
 * @param filename Name of the event stream file.
 * It should have \p .es extension.
 *
 * @return Basic statistics of the event stream file.
 */
template <sepia::type Type, typename Event>
inline StreamStatistics
stream_statistics_from_file(const std::string& filename)
{
  auto event_to_statistics = [](Event event, uint64_t t_first,
                                uint64_t number_events,
                                uint64_t duration) -> StreamStatistics {
    return {event.t, t_first, number_events, duration};
  };

  StreamStatistics stream_statistics;
  auto handle_event_stream_statistics = [&](StreamStatistics statistics) {
    stream_statistics = statistics;
  };

  auto event_stream_statistics =
      event_batch::make_event_stream_statistics<Event>(
          event_to_statistics, handle_event_stream_statistics);

  sepia::join_observable<Type>(sepia::filename_to_ifstream(filename),
                               event_stream_statistics);

  return stream_statistics;
}

/**
 * @brief Displays runtime statistics.
 *
 * @param t_diff Elapsed computation time \f$[\text{microseconds}]\f$.
 * @param stream_statistics Stream statistics \sa StreamStatistics.
 */
void
display_runtime_statistics(const double t_diff,
                           const StreamStatistics& stream_statistics)
{
  std::cout << "sequence duration: " << 1e-6 * stream_statistics.duration
            << " [sec]\n";
  std::cout << "number of events: " << stream_statistics.number_events << '\n';
  std::cout << "elapsed time: " << 1e-6 * t_diff << " [sec]\n";
  std::cout << "elapsed time per event: "
            << t_diff / stream_statistics.number_events << " [microsec]\n";
  std::cout << "real-time factor: " << stream_statistics.duration / t_diff
            << " (>1 means real-time)\n";
}
}  // namespace event_batch

#endif  // EVENT_BATCH_STREAM_STATISTICS_HPP
