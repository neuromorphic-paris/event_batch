#include "event_batch/global_decay.hpp"

#include <gtest/gtest.h>

#include "event_batch/types.hpp"

TEST(event_batch, GlobalDecay)
{
  using namespace event_batch;

  const uint64_t t_decay_first = 10000;

  Decay event_decay;
  auto handle_decay = [&](Decay decay) { event_decay = decay; };

  auto global_decay = make_global_decay<Event>(
      t_decay_first,
      [](Event event, float decay, float n_decay, float t_decay,
         float rate) -> Decay {
        return {event.t, decay, n_decay, t_decay, rate};
      },
      handle_decay);

  global_decay({0, 120, 90, 0});
  EXPECT_EQ(event_decay.t, 0);
  EXPECT_EQ(event_decay.decay, static_cast<float>(1));
  EXPECT_EQ(event_decay.n_decay, static_cast<float>(1));
  EXPECT_EQ(event_decay.t_decay, static_cast<float>(t_decay_first));
  EXPECT_EQ(event_decay.rate, static_cast<float>(1) / t_decay_first);
}
