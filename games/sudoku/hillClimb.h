#include "minorGems/util/random/RandomSource.h"



/**
 * Hill climb to look for a harder blank map.
 *
 * @param inBlankMap a map where true represents a non-blank cell.
 * @param inSolution the solution to the puzzle.
 * @param inRandomSource the random source to use.
 * @param inDesiredScore the score to stop at and call success.
 * @param inMaxTotalSteps the maximum number of steps to take (total for
 *   all levels).
 * @param inConsecutiveFailuresBeforeGiveUp how many consecutive failures
 *   to find an upward slope at this level are allowed before giving up
 *   at this level.
 *   -1 for no limit.
 *   Defaults to -1.
 *
 * @return true if we hit the desired score (and inBlankMap represents a
 *   puzzle with that score).
 *   Return false if we exhausted the climbing possibilities (or hit
 *   inConsecutiveFailuresBeforeGiveUp) at this level.
 */
char hillClimb( char inBlankMap[81], int inSolution[81],
                RandomSource *inRandomSource,
                int inDesiredScore,
                int inMaxTotalSteps,
                int inConsecutiveFailuresBeforeGiveUp = -1 );
