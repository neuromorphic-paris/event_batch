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
    float weight_thresh;
  };

  return pontella::main(
      {"runtime_batch is a runtime benchmark that estimates batches of events "
       "from an Event Stream file",
       "Usage: ./runtime_batch [options] /path/to/input.es",
       "Available options:",
       "    -t t, --time-decay-first t      sets the initial time decay",
       "                                        defaults to 10000",
       "    -e e, --weight-threshold e      sets the weight threshold",
       "                                        defaults to 0.1",
       "    -h, --help                      shows this help message"},
      argc, argv, 1, {{"time-decay-first", {"t"}}, {"weight-threshold", {"e"}}},
      {}, [&](pontella::command command) {
        const std::string& filename = command.arguments[0];
        const StreamStatistics stream_statistics =
            stream_statistics_from_file<Type, Event>(filename);

        Arguments arguments;
        arguments.t_decay_first =
            extract_argument(command, "time-decay-first", 10000);
        arguments.weight_thresh =
            extract_argument(command, "weight-threshold", 0.1);

        TicToc t;

        Decay event_decay;
        auto handle_decay = [&](Decay decay) { event_decay = decay; };

        auto global_decay = make_global_decay<Event>(
            arguments.t_decay_first,
            [](Event event, float decay, float n_decay, float t_decay,
               float rate) -> Decay {
              return {event.t, decay, n_decay, t_decay, rate};
            },
            handle_decay);

        StdVector<Event> event_batch;
        auto handle_batch = [&](StdVector<Event> batch) {
          event_batch = batch;
        };

        auto batch = make_batch<Event>(arguments.weight_thresh, event_decay,
                                       handle_batch);

        t.tic();
        sepia::join_observable<Type>(sepia::filename_to_ifstream(filename),
                                     [&](Event event) {
                                       global_decay(event);
                                       batch(event);
                                     });
        const double t_diff = t.toc<TicToc::MicroSeconds>();

        std::cout << "t first: " << event_batch.front().t
                  << ", t last: " << event_batch.back().t
                  << ", size: " << event_batch.size() << '\n';
        display_runtime_statistics(t_diff, stream_statistics);
      });
}
