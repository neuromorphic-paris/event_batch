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
      {"batch_timestamp is an executable that estimates the end timestamp "
       "[microseconds] of batches of events from an Event Stream file",
       "Usage: ./batch_timestamp [options] /path/to/input.es",
       "Available options:",
       "    -t t, --time-decay-first t      sets the initial time decay",
       "                                        defaults to 10000",
       "    -e e, --weight-threshold e      sets the weight threshold",
       "                                        defaults to 0.1",
       "    -h, --help                      shows this help message"},
      argc, argv, 1, {{"time-decay-first", {"t"}}, {"weight-threshold", {"e"}}},
      {}, [&](pontella::command command) {
        const std::string& filename = command.arguments[0];

        Arguments arguments;
        arguments.t_decay_first =
            extract_argument(command, "time-decay-first", 10000);
        arguments.weight_thresh =
            extract_argument(command, "weight-threshold", 0.1);

        Decay event_decay;
        auto handle_global_decay = [&](Decay decay) { event_decay = decay; };

        auto global_decay = make_global_decay<Event>(
            arguments.t_decay_first,
            [](Event event, float decay, float n_decay, float t_decay,
               float rate) -> Decay {
              return {event.t, decay, n_decay, t_decay, rate};
            },
            handle_global_decay);

        auto handle_batch = [](StdVector<Event> batch) {
          std::cout << batch.back().t << '\n';
        };

        auto batch = make_batch<Event>(arguments.weight_thresh, event_decay,
                                       handle_batch);

        sepia::join_observable<Type>(sepia::filename_to_ifstream(filename),
                                     [&](Event event) {
                                       global_decay(event);
                                       batch(event);
                                     });

        if (batch.batch().size() > 0)
        {
          std::cout << batch.batch().back().t << '\n';
        }
      });
}
