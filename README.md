# Draughts Engine

A C++ implementation of English draughts (checkers) with three game-playing algorithms — Minimax, Alpha-Beta Pruning, and Monte Carlo Tree Search — built alongside a benchmarking suite to measure and validate each algorithmic improvement.

This started as an NEA project (at A-Level Computer Science) and has since grown into an ongoing exploration of search optimisation, with an emphasis on measuring rather than assuming that each change helps.

## Algorithms

- **Minimax** — exhaustive game tree search, parallelised at the root using OpenMP (near-linear speedup, since root subtrees are independent)
- **Alpha-Beta Pruning** — minimax with pruning, plus an optional shallow-search move ordering pass at the root to improve cutoff rates
- **Monte Carlo Tree Search** — UCT-based tree search with random rollouts

A recursive move generator handles valid move calculation, including forced-capture rules and multi-jump sequences via backtracking.

## Benchmarking

Every algorithmic change is validated for correctness (same evaluation score or resulting board state as the baseline, on a fixed, reproducible set of positions) and measured before being merged in. Results live in `results/`, with the C++ benchmark harnesses in `source/apps/` and analysis notebooks in `analysis/`. Some harnesses used during development (e.g. for temporary classes like `MinimaxParallel`) have since been removed once their changes were merged into the main implementation. Full history is available in the git log.

Experiments so far:

| Experiment | Question | Files |
|---|---|---|
| Minimax vs Alpha-Beta parity | Do both algorithms agree, and how much does pruning save? | `benchmark.cpp` → `benchmark_results.csv` |
| Minimax vs Alpha-Beta performance | Is there a statistically significant performance difference between the two algorithms? | `minimax_vs_alpha_beta_baseline_test.cpp` → `minimax_vs_alpha_beta_baseline_results.csv` |
| Alpha-Beta vs MCTS strength | How does AB at various depths compare to MCTS? | `mcts_strength_test.cpp` → `mcts_vs_alphabeta_*.csv` |
| Minimax parallelisation | What speedup does OpenMP root-level parallelism give? | `minimax_serial_vs_parallel_test.cpp` → `minimax_serial_vs_parallel_results.csv` (harness since removed; see git history) |
| Alpha-Beta move ordering | Does shallow-search move ordering reduce nodes visited, and how close does it get to the theoretical b^(d/2) bound? | `alpha_beta_move_ordering_test.cpp` → `alpha_beta_move_ordering_test.csv` |

Correctness checks vary by experiment: earlier tests (parallel minimax) compare the resulting board state directly, while later ones (move ordering) compare the returned evaluation score, since ties in the evaluation function mean two algorithms can legitimately select different, but equally good moves.

## Project structure

```
include/        Header files
source/core/    Algorithm and engine implementations
source/apps/    Entry points: gameplay, and benchmark/test harnesses
results/        CSV output from benchmark runs
analysis/       Jupyter notebooks analysing results
```

## Debugging Flags

Can be found in the `Utilities.hpp` file. Used for testing.

> [!CAUTION]
> Do not enable in normal execution of program, as they produce huge amounts of output. Only use in custom test cases.

## Build

```bash
make        # builds every app binary into build/
make clean  # removes build artefacts
```

Requires a C++17 compiler with OpenMP support.