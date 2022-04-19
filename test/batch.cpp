#include "event_batch/batch.hpp"

#include <gtest/gtest.h>

#include "event_batch/global_decay.hpp"
#include "event_batch/types.hpp"

TEST(event_batch, Batch)
{
  using namespace event_batch;

  const uint64_t t_decay_first = 10000;
  const float weight_thresh = 1.0;

  Decay event_decay;
  auto handle_global_decay = [&](Decay decay) { event_decay = decay; };

  auto global_decay = make_global_decay<Event>(
      t_decay_first,
      [](Event event, float decay, float n_decay, float t_decay,
         float rate) -> Decay {
        return {event.t, decay, n_decay, t_decay, rate};
      },
      handle_global_decay);

  StdVector<Event> event_batch;
  auto handle_batch = [&](StdVector<Event> batch) { event_batch = batch; };

  auto batch = make_batch<Event>(weight_thresh, event_decay, handle_batch);

  {
    const Event event{0, 120, 90, 0};
    global_decay(event);
    batch(event);
    EXPECT_EQ(event_batch.size(), 0);
  }
  {
    const Event event{10, 240, 180, 1};
    global_decay(event);
    batch(event);
    EXPECT_EQ(event_batch.size(), 2);
    EXPECT_EQ(event_batch.front().t, 0);
    EXPECT_EQ(event_batch.front().x, 120);
    EXPECT_EQ(event_batch.front().y, 90);
    EXPECT_EQ(event_batch.front().p, 0);
    EXPECT_EQ(event_batch.back().t, 10);
    EXPECT_EQ(event_batch.back().x, 240);
    EXPECT_EQ(event_batch.back().y, 180);
    EXPECT_EQ(event_batch.back().p, 1);
  }
}
