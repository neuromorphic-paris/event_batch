#include "event_batch/event_stream_statistics.hpp"

#include <gtest/gtest.h>

#include "event_batch/stream_statistics.hpp"
#include "event_batch/types.hpp"

TEST(event_batch, EventStreamStatistics)
{
  using namespace event_batch;

  auto event_to_statistics = [](Event event, uint64_t t_first,
                                uint64_t number_events,
                                uint64_t duration) -> StreamStatistics {
    return {event.t, t_first, number_events, duration};
  };

  StreamStatistics stream_statistics;
  auto handle_event_stream_statistics = [&](StreamStatistics statistics) {
    stream_statistics = statistics;
  };

  auto event_stream_statistics = make_event_stream_statistics<Event>(
      event_to_statistics, handle_event_stream_statistics);

  event_stream_statistics({10, 120, 90, 0});
  EXPECT_EQ(stream_statistics.t, 10);
  EXPECT_EQ(stream_statistics.t_first, 10);
  EXPECT_EQ(stream_statistics.number_events, 1);
  EXPECT_EQ(stream_statistics.duration, 0);

  event_stream_statistics({20, 120, 90, 0});
  EXPECT_EQ(stream_statistics.t, 20);
  EXPECT_EQ(stream_statistics.t_first, 10);
  EXPECT_EQ(stream_statistics.number_events, 2);
  EXPECT_EQ(stream_statistics.duration, 10);

  event_stream_statistics({50, 120, 90, 0});
  EXPECT_EQ(stream_statistics.t, 50);
  EXPECT_EQ(stream_statistics.t_first, 10);
  EXPECT_EQ(stream_statistics.number_events, 3);
  EXPECT_EQ(stream_statistics.duration, 40);
}
