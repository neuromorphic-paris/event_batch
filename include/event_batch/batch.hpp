/**
 * @file
 * @brief Batch estimator from global decay implementation.
 */

#ifndef EVENT_BATCH_BATCH_HPP
#define EVENT_BATCH_BATCH_HPP

#include <utility>

#include "event_batch/global_decay.hpp"
#include "event_batch/types.hpp"

namespace event_batch
{
/**
 * @brief Batch estimator from global decay.
 *
 * This class estimates the ideal batch of a stream of events from the
 * corresponding global decay.
 *
 * @tparam Event Type of event.
 * @tparam HandleBatch Type of the handle to further process the estimated
 * batch.
 */
template <typename Event, typename HandleBatch>
class Batch
{
 public:
  /**
   * @brief Constructs an instance to estimate the ideal batch of a stream of
   * events from the corresponding global decay.
   *
   * @param weight_thresh @copybrief weight_thresh_
   * @param decay @copybrief decay_
   * @param handle_batch @copybrief handle_batch_
   */
  Batch(const float weight_thresh, const Decay& decay,
        HandleBatch&& handle_batch)
      : weight_thresh_(weight_thresh),
        decay_(decay),
        handle_batch_(std::forward<HandleBatch>(handle_batch))
  {
  }
  /**
   * @brief Deleted copy constructor.
   */
  Batch(const Batch&) = delete;
  /**
   * @brief Default move constructor.
   */
  Batch(Batch&&) = default;
  /**
   * @brief Deleted copy assignment operator.
   */
  Batch&
  operator=(const Batch&) = delete;
  /**
   * @brief Default move assignment operator.
   */
  Batch&
  operator=(Batch&&) = default;
  /**
   * @brief Default destructor.
   */
  ~Batch() = default;

  /**
   * @brief Estimates the ideal batch of a stream of events from the
   * corresponding global decay one event at a time.
   *
   * This method estimates the ideal batch of a stream of events from the
   * corresponding global decay in an event-by-event basis.
   *
   * @param event Incoming event.
   */
  void
  operator()(Event event)
  {
    batch_.push_back(event);

    const float t_diff =
        (event.t > batch_[0].t) ? static_cast<float>(event.t - batch_[0].t) : 0;
    const float weight = static_cast<float>(1) /
                         (static_cast<float>(1e-6) * t_diff * decay_.n_decay +
                          static_cast<float>(1));

    if (weight < weight_thresh_)
    {
      handle_batch_(std::move(batch_));
    }
  }

  /**
   * @brief Resets the context.
   */
  void
  reset()
  {
    batch_.clear();
  }

 protected:
  /**
   * @brief Weight threshold that splits the batches.
   */
  const float weight_thresh_;

  /**
   * @brief Decay stucture.
   * \sa event_batch::Decay.
   */
  const Decay& decay_;

  /**
   * @brief Event batch.
   */
  StdVector<Event> batch_;

  /**
   * @brief Handle to further process the estimated batch.
   */
  HandleBatch handle_batch_;
};

/**
 * @brief Make function that creates an instance of event_batch::Batch.
 *
 * @tparam Event Type of event.
 * @tparam HandleBatch Type of the handle to further process the estimated
 * batch.
 *
 * @param weight_thresh Weight threshold that splits the batches.
 * @param decay Decay stucture.
 * \sa event_batch::Decay.
 * @param handle_batch Handle to further process the estimated batch.
 *
 * @return Instance of event_batch::GlobalDecay.
 */
template <typename Event, typename HandleBatch>
inline Batch<Event, HandleBatch>
make_batch(const float weight_thresh, const Decay& decay,
           HandleBatch&& handle_batch)
{
  return Batch<Event, HandleBatch>(weight_thresh, decay,
                                   std::forward<HandleBatch>(handle_batch));
}
}  // namespace event_batch

#endif  // EVENT_BATCH_BATCH_HPP
