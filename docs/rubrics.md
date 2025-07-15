Evaluation Metrics

Each .c file is scored on the following criteria:

1. Compilation (40%)

Does the driver compile cleanly?

Are there any errors or warnings?

Is a Makefile present?

2. Security & Safety (25%)

Does the driver use copy_to_user / copy_from_user?

Are mutexes/spinlocks used to prevent race conditions?

Is input validation implemented?

3. Code Quality (20%)

Kernel coding style compliance

Function documentation using comments

Minimal use of goto

4. Performance (10%)

Efficient code structure (measured by line count)

No redundant loops or CPU-intensive logic

5.  Advanced Features (5%)

Presence of debugging support like printk, pr_debug, or #ifdef DEBUG

Final Score = Weighted Average

The final score is a sum of all weighted components:
score = 0.4*Compilation + 0.25*Security + 0.2*Quality + 0.1*Performance + 0.05*Debugging