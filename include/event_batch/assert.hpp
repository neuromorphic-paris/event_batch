/**
 * @file
 * @brief Custom assert message for debug purposes.
 */

#ifndef EVENT_BATCH_ASSERT_HPP
#define EVENT_BATCH_ASSERT_HPP

#include <exception>
#include <iostream>

namespace event_batch
{
/// \cond
#ifndef NDEBUG
#define ASSERT(condition, message)                                     \
  do                                                                   \
  {                                                                    \
    if (!(condition))                                                  \
    {                                                                  \
      std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                << " line " << __LINE__ << ": " << message << '\n';    \
      std::terminate();                                                \
    }                                                                  \
  } while (false)
#else
#define ASSERT(condition, message) \
  do                               \
  {                                \
  } while (false)
#endif
/// \endcond
}  // namespace event_batch

#endif  // EVENT_BATCH_ASSERT_HPP
