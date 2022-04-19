#include <string>

#include "event_batch.hpp"
#include "pontella.hpp"
#include "sepia.hpp"

int
main(int argc, char* argv[])
{
  using namespace event_batch;

  typedef sepia::dvs_event Event;
  constexpr sepia::type Type = sepia::type::dvs;

  struct Arguments
  {
    uint64_t t_decay_first;
  };

  return pontella::main(
      {"runtime_global_decay is a runtime benchmark that computes the global "
       "decay from an Event Stream file",
       "Usage: ./runtime_global_decay [options] /path/to/input.es",
       "Available options:",
       "    -t t, --time-decay-first t      sets the initial time decay",
       "                                        defaults to 10000",
       "    -h, --help                      shows this help message"},
      argc, argv, 1, {{"time-decay-first", {"t"}}}, {},
      [](pontella::command command) {
        const std::string& filename = command.arguments[0];
        const StreamStatistics stream_statistics =
            stream_statistics_from_file<Type, Event>(filename);

        Arguments arguments;
        arguments.t_decay_first =
            extract_argument(command, "time-decay-first", 10000);

        TicToc t;

        Decay event_decay;
        auto handle_global_decay = [&](Decay decay) { event_decay = decay; };

        auto global_decay = make_global_decay<Event>(
            arguments.t_decay_first,
            [](Event event, float decay, float n_decay, float t_decay,
               float rate) -> Decay {
              return {event.t, decay, n_decay, t_decay, rate};
            },
            handle_global_decay);

        t.tic();
        sepia::join_observable<Type>(sepia::filename_to_ifstream(filename),
                                     global_decay);
        const double t_diff = t.toc<TicToc::MicroSeconds>();

        std::cout << "t: " << event_decay.t << ", decay: " << event_decay.decay
                  << ", n decay: " << event_decay.n_decay
                  << ", t decay: " << event_decay.t_decay
                  << ", rate: " << event_decay.rate << '\n';
        display_runtime_statistics(t_diff, stream_statistics);
      });
}
