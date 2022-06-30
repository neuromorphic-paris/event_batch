# event_batch

[![Build Status](https://app.travis-ci.com/neuromorphic-paris/event_batch.svg?token=EcsqzkxC73oqb86Uxx4r&branch=master)](https://app.travis-ci.com/github/neuromorphic-paris/event_batch)
[![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg?style=flat-square)](https://creativecommons.org/licenses/by-nc-sa/4.0/)

The authors provide this code in the hope it will be useful for understanding the proposed method, as well as for reproducibility of the results.

For more information and more open-source software please visit the neuromorphic-paris' Github page: <https://github.com/neuromorphic-paris>.

## Requirements

This code was tested on Ubuntu 20.04 distro.

### Dependencies

- CMake: <https://cmake.org/download/>

```bash
sudo apt install cmake cmake-curses-gui
```

- doxygen (optional, only needed if documentation is enabled): <https://www.doxygen.nl>

The following are dependencies that are automatically downloaded during the build process, so you do not need to explicitly install them.

- pontella: <https://github.com/neuromorphic-paris/pontella>

- sepia: <https://github.com/neuromorphic-paris/sepia>

- googletest (only downloaded if tests are enabled): <https://github.com/google/googletest.git>

### General

```bash
git clone https://github.com/neuromorphic-paris/event_batch.git
cd event_batch
mkdir build && cd build
cmake .. <cmake arguments>
```

The cmake arguments can be set as follows:

```bash
-Devent_batch_BUILD_DOC=ON/OFF
                            Build documentation.
                            (default: OFF)
-Devent_batch_BUILD_RUNTIME_BENCHMARK=ON/OFF
                            Build runtime benchmark.
                            (default: OFF)
-Devent_batch_BUILD_TEST=ON/OFF
                            Build tests.
                            (default: OFF)
```

Lastly, ensure all environment path variables are well set, and compile everything:

```bash
make
```

For efficiency reasons, the input events are assumed to be in the [Event Stream](https://github.com/neuromorphic-paris/event_stream) format.
Please refer to the [loris](https://github.com/neuromorphic-paris/loris) library to convert to/from the [Event Stream](https://github.com/neuromorphic-paris/event_stream) format.

## Usage

Two executables are provided that estimate the batch size ([batch_size.cpp](https://github.com/neuromorphic-paris/event_batch/blob/master/src/batch_size.cpp)) of an event stream or the end timestamp [microseconds] of the batch ([batch_timestamp.cpp](https://github.com/neuromorphic-paris/event_batch/blob/master/src/batch_timestamp.cpp)).
Assuming you are in the `build` directory, to run an executable `*`, just run on a terminal:

```bash
./src/batch_* [options] /path/to/input.es
```

where `*` is either `size` or `timestamp`.

The estimates are sent via the standard output, so you can redirect them with the pipe operator `|` to another executable, e.g.:

```bash
./src/batch_* [options] /path/to/input.es | ./your/executable
```

## Runtime Benchmark

The runtime benchmark can be built by setting the flag `event_batch_BUILD_RUNTIME_BENCHMARK` to `ON`.
Assuming you are in the `build` directory, to run a runtime executable `*`, just run on a terminal:

```bash
./src/runtime_benchmark/runtime_* [options] /path/to/input.es
```

## Tests

The test suite can be built by setting the flag `event_batch_BUILD_TEST` to `ON`.
To run the test suite, just run on a terminal:

```bash
make test
```

## Documentation

Offline documentation can be built with [doxygen](https://www.doxygen.nl) by setting the flag `event_batch_BUILD_DOC` to `ON`.
Then, just run on a terminal:

```bash
make doc
```

## License

The EventEMin code is licensed under [CC BY-NC-SA 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/).
Commercial usage is not permitted.
