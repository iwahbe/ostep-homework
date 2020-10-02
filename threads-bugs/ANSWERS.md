# HW Answers

1. The output is consistent between runs.
2. The code does not deadlock ever. I tried with `-l 5 -d -n 2 -v`. I tried again with `-l 100000`, the code deadlocks often.
3. I see no deadlocks on `-n 2`, but deadlocks on lots of other stuff.
4. The code imposes a partial ordering on the elements. There is a special case
   because if both vectors are the same (and thus have the same lock), not
   checking would attempt to acquire the same lock twice, which guarantees a
   deadlock.
5. `time ./vector-global-order -t -n 2 -l 100000 -d` = 0.023s, `time ./vector-global-order -t -n 2 -l 200000 -d` = 0.043s, `time ./vector-global-order -t -n 3 -l 200000 -d` = 0.155s
6. The number of threads stops increasing time linearly. `time ./vector-global-order -t -n 8 -l 200000 -d -p` = 0.062s
7. The first call is necessary if the vectors are not always in the same order. It's much slower
   `time ./vector-try-wait -t -n 8 -l 200000 -d` = 8.90s compared to 1.78s on global order
8. The main problem is a unnecessary and expensive global lock. `time ./vector-avoid-hold-and-wait -t -n 8 -l 200000 -d` = 4.52s while `time ./vector-avoid-hold-and-wait -t -n 8 -l 200000 -d -p` = 0.28s Its reasonably
   fast in both scenarios, but the global-ordering strategy is faster still.
9. The semantics look similar, but I'm not sure. The semantics are not exactly
   the same because they don't use the locking component of a vector. This also
   allows interleaving of adds, but since `a + b = b + a`, this shouldn't
   matter.
10. `time ./vector-nolock -t -n 2 -l 200000 -d -p` = 0.09s, `time ./vector-nolock -t -n 2 -l 200000 -d` = 0.88s. It's not that fast.
