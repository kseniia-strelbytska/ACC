## Aim

This project investigates the extent to which **algorithmic and heuristic search methods can provide credible empirical evidence for or against the Andrews–Curtis conjecture**, focusing on balanced presentations of the trivial group. Rather than attempting a proof, the goal is to evaluate how far computational approaches can go in reproducing, extending, and analysing known positive results—and where they begin to fail.

## Methods

I implemented a suite of **independent search algorithms from scratch in C++**, including greedy best-first search, Dijkstra-style baselines, and enhanced variants using **rotation invariants, ranked super-moves (insert–rotate transformations), and neighbourhood-guided exploration**. The algorithms operate directly on Nielsen/Andrews–Curtis moves and their extensions, with explicit control over branching factor, relator length bounds, and node expansion limits. All implementations were cross-validated against a Python reference version, with extensive logging used to diagnose correctness and performance differences.

## Results and Novel Contributions

As a benchmark, the implementation successfully trivialises **533 out of the full 1190 Miller–Schupp presentations under the classical ordering**, matching one of the strongest known computational results reported in the literature. Beyond reproducing these results, the project explores **non-classical move rankings, rotation-invariant normal forms, and insert–rotate super-moves** to reduce search depth and eliminate redundant exploration. Additional analysis examines the structure of trivialisation paths and the distribution of “easy” versus “hard” instances.

## Heuristics, Hypotheses, and Limitations

The project further investigates **heuristic quality and structural signals of difficulty**, including edit distance, cyclic q-gram similarity, longest common substring metrics, and rotation-invariant representations. While these measures capture some structural information, their correlation with true trivialisation distance is consistently weak. This suggests that although algorithmic methods can reliably reproduce known positive cases, **heuristic guidance remains fundamentally limited**, constraining the strength of computational evidence for the conjecture.

## Status

This project is **ongoing**.
