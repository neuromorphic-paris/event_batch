#include <string>

#include "event_batch.hpp"
#include "pontella.hpp"
#include "sepia.hpp"

int
main(int argc, char* argv[])
{
  using namespace event_batch;

  typedef sepia::dvs_event Event;

  return pontella::main(
      {"runtime_event_stream_statistics is a runtime benchmark that computes "
       "the number of events and duration from an Event Stream file",
       "Usage: ./runtime_event_stream_statistics [options] /path/to/input.es",
       "Available options:",
       "    -h, --help                 shows this help message"},
      argc, argv, 1, {}, {}, [](pontella::command command) {
        const std::string& filename = command.arguments[0];

        TicToc t;

        t.tic();
        const StreamStatistics stream_statistics =
            stream_statistics_from_file<sepia::type::dvs, Event>(filename);
        const double t_diff = t.toc<TicToc::MicroSeconds>();

        std::cout << "t: " << stream_statistics.t
                  << ", t first: " << stream_statistics.t_first << '\n';
        display_runtime_statistics(t_diff, stream_statistics);
      });
}
