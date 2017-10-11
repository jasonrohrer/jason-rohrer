#include "minorGems/util/random/RandomSource.h"



/**
 * Recursively adds values to a puzzle, searching for a hard
 * puzzle.
 *
 * @param inPuzzle the puzzle to add values to.
 *   Destroyed by caller.
 * @param inLowScore the lowest acceptable score.
 * @param inHighScore the lowest acceptable score.
  * @param inRandomSource the source of randomness.
 *   Destroyed by caller.
 *
 * @return 1 inPuzzle now contains a puzzle with a hardness score in
 *   range, or -2 if a contradiction found or
 *   -1 if all possible more-full puzzles have hardness scores that are out
 *   of range.
 */
int add( int *inPuzzle, int inLowScore, int inHighScore,
         RandomSource *inRandomSource  );




/**
 * Similar to add, but doesn't look for hard puzzles.  Instead, continues
 * until entire grid is full.
 */
void addToFull( int *inPuzzle, RandomSource *inRandomSource );

