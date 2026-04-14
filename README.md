*This project has been created as part of the 42 curriculum by rumontei.*

# Codexion

## Description
Codexion is a concurrency simulation project inspired by the classic dining-philosophers problem, adapted to a software-engineering context.

Each coder thread repeatedly performs a cycle:
1. take two shared dongles,
2. compile,
3. debug,
4. refactor.

The project goal is to implement correct and deterministic multithreaded behavior under contention, with support for multiple scheduling policies:
- **FIFO** (first requester proceeds first),
- **EDF** (Earliest Deadline First), where `deadline = last_compile_start + time_to_burnout`.

The simulation stops when one of these conditions is met:
- one coder burns out (`time since last compile start > time_to_burnout`), or
- all coders have compiled at least `number_of_compiles_required` times.

## Instructions
### Requirements
- Linux/macOS environment
- `gcc`
- `make`
- `pthread` support

### Build
```bash
make
```

### Clean
```bash
make clean
make fclean
make re
```

### Run
```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

Where:
- `number_of_coders`: number of coder threads
- `time_to_burnout`: max time (ms) without starting a new compile
- `time_to_compile`: compile duration (ms)
- `time_to_debug`: debug duration (ms)
- `time_to_refactor`: refactor duration (ms)
- `number_of_compiles_required`: stop condition target for each coder
- `dongle_cooldown`: cooldown (ms) before a dongle can be reused
- `scheduler`: `fifo` or `edf`

### Usage examples
```bash
./codexion 3 2000 150 50 50 2 100 fifo
./codexion 3 2000 150 50 50 2 100 edf
```

### Optional memory check
```bash
valgrind --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all ./codexion 3 1200 100 80 60 2 50 fifo
```

## Blocking cases handled
- **Deadlock prevention:** lock ordering and scheduling constraints avoid circular wait on dongles; FIFO/EDF gate access before dual-dongle acquisition.
- **Coffman conditions handling:** mutual exclusion is required for dongles, but hold-and-wait/circular wait are mitigated by controlled acquisition strategy and scheduler coordination.
- **Starvation mitigation:**
  - FIFO mode preserves arrival order fairness,
  - EDF mode prioritizes the coder with the earliest burnout deadline.
- **Cooldown contention handling:** each dongle has `last_use`, and reuse is blocked until `dongle_cooldown` has elapsed.
- **Precise burnout detection:** a dedicated monitor thread checks each coder’s `last_compile_time` against `time_to_burnout`.
- **Serialized logging:** output is protected by a logging mutex to avoid interleaved and corrupted logs.

## Thread synchronization mechanisms
This project uses standard pthread primitives:

- **`pthread_mutex_t`**
  - dongle mutexes protect dongle ownership and cooldown state,
  - per-coder mutexes protect `last_compile_time` and compile counters,
  - global mutexes protect stop state and logging.

- **`pthread_cond_t`**
  - used in FIFO mode with a ticket mechanism (`next_ticket` / `serving_ticket`) to wake waiting coders in order.

- **Custom scheduling gate (FIFO/EDF)**
  - coder threads call a turn-selection step before taking dongles,
  - monitor and workers communicate through shared state protected by mutexes.

### Race-condition prevention examples
- `last_compile_time` is always read/written under `last_compile_mutex`.
- compile counters are updated and read under `compiles_mutex`.
- stop flag is accessed under `stop_mutex`.
- logs are printed under `log_mutex`, ensuring thread-safe, non-overlapping output.

## Resources
### Concurrency and pthread references
- POSIX Threads Programming: https://man7.org/linux/man-pages/man7/pthreads.7.html
- Mutexes: https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html
- Condition variables: https://man7.org/linux/man-pages/man3/pthread_cond_wait.3p.html
- Classic dining philosophers background:
  - https://en.wikipedia.org/wiki/Dining_philosophers_problem

### Scheduling references
- Earliest Deadline First (EDF):
  - https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling

### How AI was used
AI assistance was used as a development support tool for:
- refactoring and code organization (splitting modules, reducing long functions to satisfy norm constraints),
- drafting and validating cleanup strategy for dynamic memory and mutex/cond destruction,
- suggesting targeted test scenarios for FIFO/EDF behavior and valgrind leak checks,

All final implementation decisions, integration, and verification were validated in-project via `make`, runtime executions, norm checks, and valgrind outputs.
