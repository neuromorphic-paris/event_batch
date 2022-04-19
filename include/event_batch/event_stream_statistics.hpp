/**
 * @file
 * @brief Implementation of basic event stream statistics.
 */

#ifndef EVENT_BATCH_EVENT_STREAM_STATISTICS_HPP
#define EVENT_BATCH_EVENT_STREAM_STATISTICS_HPP

#include <cstdint>
#include <utility>

#include "event_batch/assert.hpp"

namespace event_batch
{
/**
 * @brief Basic event stream statistics.
 *
 * This class computes basic statistics of an event stream.
 * Currently, it stores the timestamp of the first event, and computes the
 * number of events and duration of the event stream.
 *
 * @tparam Event Type of event.
 * @tparam EventToStatistics Type of the handle to pass from an event to the
 * event stream statistics.
 * @tparam HandleStatistics Type of the handle to further process the
 * computed statistics.
 */
template <typename Event, typename EventToStatistics, typename HandleStatistics>
class EventStreamStatistics
{
 public:
  /**
   * @brief Constructs an instance to compute basic statistics of an event
   * stream.
   *
   * @param event_to_statistics @copybrief event_to_statistics_
   * @param handle_statistics @copybrief handle_statistics_
   */
  EventStreamStatistics(EventToStatistics&& event_to_statistics,
                        HandleStatistics&& handle_statistics)
      : t_first_(0),
        number_events_(0),
        first_(true),
        event_to_statistics_(
            std::forward<EventToStatistics>(event_to_statistics)),
        handle_statistics_(std::forward<HandleStatistics>(handle_statistics))
  {
  }
  /**
   * @brief Deleted copy constructor.
   */
  EventStreamStatistics(const EventStreamStatistics&) = delete;
  /**
   * @brief Default move constructor.
   */
  EventStreamStatistics(EventStreamStatistics&&) = default;
  /**
   * @brief Deleted copy assignment operator.
   */
  EventStreamStatistics&
  operator=(const EventStreamStatistics&) = delete;
  /**
   * @brief Default move assignment operator.
   */
  EventStreamStatistics&
  operator=(EventStreamStatistics&&) = default;
  /**
   * @brief Default destructor.
   */
  ~EventStreamStatistics() = default;

  /**
   * @brief Computes the basic statistics one event at a time.
   *
   * This method computes the number of events and duration of an event stream
   * in an event-by-event basis.
   *
   * @param event Incoming event.
   */
  void
  operator()(Event event)
  {
    ASSERT(event.t >= t_first_, "The current timestamp "
                                    << event.t << "must be >= first timestamp "
                                    << t_first_);

    if (first_)
    {
      t_first_ = event.t;
      first_ = false;
    }
    ++number_events_;

    handle_statistics_(event_to_statistics_(event, t_first_, number_events_,
                                            event.t - t_first_));
  }

 protected:
  /**
   * @brief First timestamp \f$[\text{microseconds}]\f$.
   */
  uint64_t t_first_;
  /**
   * @brief Number of events.
   */
  uint64_t number_events_;
  /**
   * @brief Flag to determine whether an event is the first of the stream or
   * not.
   */
  bool first_;

  /**
   * @brief Handle to pass from an event to the event stream statistics.
   */
  EventToStatistics event_to_statistics_;
  /**
   * @brief Handle to further process the computed statistics.
   */
  HandleStatistics handle_statistics_;
};

/**
 * @brief Make function that creates an instance of
 * event_batch::EventStreamStatistics.
 *
 * @tparam Event Type of event.
 * @tparam EventToStatistics Type of the handle to pass from an event to the
 * event stream statistics.
 * @tparam HandleStatistics Type of the handle to further process the
 * computed statistics.
 *
 * @param event_to_statistics Handle to pass from an event to the event stream
 * statistics.
 * @param handle_statistics Handle to further process the computed statistics.
 *
 * @return Instance of event_batch::EventStreamStatistics.
 */
template <typename Event, typename EventToStatistics, typename HandleStatistics>
inline EventStreamStatistics<Event, EventToStatistics, HandleStatistics>
make_event_stream_statistics(EventToStatistics&& event_to_statistics,
                             HandleStatistics&& handle_statistics)
{
  return EventStreamStatistics<Event, EventToStatistics, HandleStatistics>(
      std::forward<EventToStatistics>(event_to_statistics),
      std::forward<HandleStatistics>(handle_statistics));
}
}  // namespace event_batch

#endif  // EVENT_BATCH_EVENT_STREAM_STATISTICS_HPP
