/**
 * @file
 * @brief Utility functions.
 */

#ifndef EVENT_BATCH_UTILS_HPP
#define EVENT_BATCH_UTILS_HPP

#include <sstream>
#include <string>

#include "pontella.hpp"

namespace event_batch
{
/**
 * @brief Extracts the argument value from a command.
 *
 * This function returns the argument value from a \p command given its \p name.
 * In case the argument \p name is not found, the default value is returned.
 *
 * @tparam T Type of the argument value.
 *
 * @param command Command line.
 * @param name Name of the argument.
 * @param default_argument Default argument value.
 *
 * @return Argument value if the argument \p name is found in the \p command.
 * @return Passed argument value if the argument \p name is not found in the \p
 * command.
 */
template <typename T>
T
extract_argument(pontella::command command, const std::string& name,
                 T default_argument)
{
  const auto name_and_argument = command.options.find(name);
  if (name_and_argument != command.options.end())
  {
    std::stringstream name_stream(name_and_argument->second);
    T argument;
    name_stream >> argument;
    return argument;
  }
  return default_argument;
}
}  // namespace event_batch

#endif  // EVENT_BATCH_UTILS_HPP
