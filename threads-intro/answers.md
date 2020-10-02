# Answers

1. We see that `dx` is initialized to 0 (after checking), then 1 is subtracted from it, and it equals -1 until exit.
2. We will see `dx` count down to -1, then exit. Multiple threads are not relevant, as all data is stored in registers.
3. Nothing changes, because the values of `dx` are still thread local.
4. We see `ax` start at 0, move to 1, and then we halt.
5. Each thread loops 3 times, because the loop counter `bx` is thread local. The final value of `value` is 6.
6. I can look at the thread interleaving. The timing of the interrupt matters a great deal. The critical section is between `mov`s.
7. The program gives the expect answer for `-i 3` or any greater value. This allows the program to exit the critical section before a context switch.
8. We see `value` is 100. For `-i x` where `x` is a multiple of 3, we see the correct value.
9. We should see the signaler thread mov `$1` to `value`, then the waiter will look at `$1` until it tests positive. The location is being used as a lock. `value`'s value should be whatever `$1` is, as moved by `.signaler`.
10. Thread 0 is waiting, so essentially just performing a spin lock. The program has no critical sections, so nothing changes. The program is maxing out the CPU, but with a spin lock, not real work.
