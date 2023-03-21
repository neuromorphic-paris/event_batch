#include <string>

#include "event_batch.hpp"
#include "pontella.hpp"
#include "select_rectangle.hpp"
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
    uint16_t left;
    uint16_t right;
    uint16_t bottom;
    uint16_t top;
  };

  return pontella::main(
      {"batch_size is an executable that estimates the size of batches of "
       "events from an Event Stream file",
       "Usage: ./batch_size [options] /path/to/input.es", "Available options:",
       "    -t t, --time-decay-first t      sets the initial time decay",
       "                                        defaults to 10000",
       "    -e e, --weight-threshold e      sets the weight threshold",
       "                                        defaults to 0.1",
       "    -cl cl, --crop-left cl          sets the crop's left side "
       "coordinate",
       "                                        defaults to 0",
       "    -cr cr, --crop-right cr         sets the crop's right side "
       "coordinate",
       "                                        defaults to context width",
       "    -cb cb, --crop-bottom cb        sets the crop's bottom side "
       "coordinate",
       "                                        defaults to 0",
       "    -ct ct, --crop-top ct           sets the crop's top side "
       "coordinate",
       "                                        defaults to context height",
       "    -h, --help                      shows this help message"},
      argc, argv, 1,
      {{"time-decay-first", {"t"}},
       {"weight-threshold", {"e"}},
       {"crop-left", {"cl"}},
       {"crop-right", {"cr"}},
       {"crop-bottom", {"cb"}},
       {"crop-top", {"ct"}}},
      {}, [&](pontella::command command) {
        const std::string& filename = command.arguments[0];
        const auto header =
            sepia::read_header(sepia::filename_to_ifstream(filename));

        Arguments arguments;
        arguments.t_decay_first =
            extract_argument(command, "time-decay-first", 10000);
        arguments.weight_thresh =
            extract_argument(command, "weight-threshold", 0.1);
        arguments.left = extract_argument(command, "crop-left", 0);
        arguments.right = extract_argument(command, "crop-right", header.width);
        arguments.bottom = extract_argument(command, "crop-bottom", 0);
        arguments.top = extract_argument(command, "crop-top", header.height);

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
          std::cout << batch.size() << '\n';
        };

        auto batch = make_batch<Event>(arguments.weight_thresh, event_decay,
                                       handle_batch);

        auto crop = tarsier::make_select_rectangle<Event>(
            arguments.left, arguments.bottom, arguments.right - arguments.left,
            arguments.top - arguments.bottom, [&](Event event) {
              global_decay(event);
              batch(event);
            });

        sepia::join_observable<Type>(sepia::filename_to_ifstream(filename),
                                     crop);

        if (batch.batch().size() > 0)
        {
          std::cout << batch.batch().size() << '\n';
        }
      });
}
