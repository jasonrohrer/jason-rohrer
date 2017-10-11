#include <stdlib.h>


/**
 * Solves a sudoku puzzle.
 *
 * @param inPuzzle the puzzle as a 81-element array with -1 representing
 *   blanks.
 *   Destroyed by caller.
 *   Note that inPuzzle will be modified by this function.
 * @param inPrintReport true to print a solver report.
 *   False to print nothing.
 * @param outHardIndices pointer to an 81-element array where indices
 *   of hard cells should be returned if solving the puzzle fails.
 *   The indices in the array are sorted with the hardest indices first.
 *   Destroyed by caller.
 * @param inStopOnProgress true to stop after one step of progress.
 *   Defaults to false.
 *
 * @return the hardness of the puzzle, -1 if solving failed, or -2 if
 *   a contradiction was discovered.
 */
int solve( int *inPuzzle, char inPrintReport = false,
           int *outHardIndices = NULL,
           char inStopOnProgress = false );



/**
 * Same as solve, but doesn't modify inPuzzle and simply generates a score
 * or result value.
 */
int score( int *inPuzzle, char inPrintReport = false );
