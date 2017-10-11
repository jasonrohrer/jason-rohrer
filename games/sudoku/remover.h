
#include "minorGems/util/random/RandomSource.h"



/**
 * Recursively removes values from a puzzle, searching for the hardest
 * puzzle.
 *
 * @param inPuzzle the puzzle to remove values from.
 *   Destroyed by caller.
 * @param inRandomSource the source of randomness.
 *   Destroyed by caller.
 *
 * @return the hardest score discovered, or -1 if the puzzle is unsolvable.
 */
int remove( int *inPuzzle, RandomSource *inRandomSource );



/**
 * Iteratively removes values from randomly-chosen cells until none
 * of the cells can be removed (removing one more leads to an unsolvable
 * puzzle).
 *
 * @param inPuzzle the puzzle to remove values from.
 *   Destroyed by caller.
 * @param inRandomSource the source of randomness.
 *   Destroyed by caller.
 * @param inMaxBlank the maximum number of cells to try blanking.
 */
void fastRemove( int *inPuzzle, RandomSource *inRandomSource,
                 int inMaxBlank );


/**
 * Fills a partial puzzle until it is solvable.
 *
 * @param inPuzzle may be partially filled or blank.
 *  On return, will be fill to a point where it is solvable.
 *   Destroyed by caller.
 * @param inSolution must be a full solution grid.
 *   Destroyed by caller.
 * @param inRandomSource the source of randomness.
 *   Destroyed by caller.
 */
void fastFill( int *inPuzzle, int *inSolution,
               RandomSource *inRandomSource );

