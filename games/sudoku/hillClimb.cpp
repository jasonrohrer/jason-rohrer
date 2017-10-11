#include <stdio.h>
#include <time.h>

#include "hillClimb.h"
#include "sudoku.h"
#include "solver.h"
#include "remover.h"


/**
 * Similar to hillClimb, but recursively calls itself.
 *
 * Extra parameters:
 * @param inTotalSteps pointer to where the overall step counter is kept
 * @param inBestBlankMapSeen location where best blank map ever discovered
 *   should be tracked.
 * @param inBestScoreSeen pointer to where best score ever seen (corresponding
 *   to best blank map) is stored.
 */
char hillClimbRecursive( char inBlankMap[81], int *inSolution,
                         RandomSource *inRandomSource,
                         int inDesiredScore,
                         int inMaxTotalSteps,
                         int inConsecutiveFailuresBeforeGiveUp,
                         int *inTotalSteps,
                         char inBestBlankMapSeen[81], int *inBestScoreSeen ) {

    // only act on the step counter here, when we know we have a solvable
    // puzzle
    
    if( *inTotalSteps >= inMaxTotalSteps ) {
        return false;
        }

    
    // how hard is the current puzzle?

    int puzzle[81];
    int i, j;

    int blankCount = 0;
    
    for( i=0; i<81; i++ ) {
        if( inBlankMap[i] ) {
            puzzle[i] = inSolution[i];
            }
        else {
            puzzle[i] = -1;
            blankCount++;
            }
        }
    
    int thisPuzzleScore = score( puzzle );


    if( thisPuzzleScore > *inBestScoreSeen ) {
        // new best overall score
        *inBestScoreSeen = thisPuzzleScore;

        for( i=0; i<81; i++ ) {
            inBestBlankMapSeen[i] = inBlankMap[i];
            }
        }
    
    
    if( thisPuzzleScore >= inDesiredScore ) {
        // hard enough
        return true;
        }
    
    
    char tryMapPair[81][81];
    int tryCountPair = 0;
    
    for( i=0; i<81; i++ ) {
        for( j=0; j<81; j++ ) {
            tryMapPair[i][j] = false;
            }
        }
    
    // first, mark all tryMap[i][i] as tried
    for( i=0; i<81; i++ ) {
        tryMapPair[i][i] = true;
        tryCountPair++;
        }

    int maxTriesPair = 81 * 81;
    

    int hardestScoreSeen = thisPuzzleScore;

    // how many tries we can make, without progress, before giving up
    // at this level
    int triesSinceProgress = 0;

    int tryWithoutProgressLimit = inConsecutiveFailuresBeforeGiveUp;
    if( tryWithoutProgressLimit == -1 ) {
        tryWithoutProgressLimit = maxTriesPair;
        }
    
    while( tryCountPair < maxTriesPair && 
           triesSinceProgress < tryWithoutProgressLimit ) {
    

        i = inRandomSource->getRandomBoundedInt( 0, 80 );
        j = inRandomSource->getRandomBoundedInt( 0, 80 );
        
        if( !tryMapPair[i][j] ) {
            // untried pair

            
                // swap their blank status
                inBlankMap[i] = ! inBlankMap[i];
                inBlankMap[j] = ! inBlankMap[j];
                
                // generate a puzzle with new map

                int newPuzzle[81];
                
                for( int c=0; c<81; c++ ) {
                    if( inBlankMap[c] ) {
                        newPuzzle[c] = inSolution[c];
                        }
                    else {
                        newPuzzle[c] = -1;
                        }
                    }
    
                int newPuzzleScore = solve( newPuzzle );

                // increment step counter here
                *inTotalSteps = *inTotalSteps + 1;
                                
                if( newPuzzleScore > thisPuzzleScore ) {
                    
                    // improvement
                    triesSinceProgress = 0;
                    
                    //printf( "New score %d\n", newPuzzleScore );
                    hardestScoreSeen = newPuzzleScore;
                    
                    // recurse

                    char result =
                        hillClimbRecursive(
                            inBlankMap, inSolution, inRandomSource,
                            inDesiredScore,
                            inMaxTotalSteps,
                            inConsecutiveFailuresBeforeGiveUp,
                            inTotalSteps,
                            inBestBlankMapSeen,
                            inBestScoreSeen );
                    
                    if( result ) {
                        return true;
                        }
                    else if( *inTotalSteps >= inMaxTotalSteps ) {
                        // hit limit
                        return false;
                        }
                    else {
                        // we found nothing by exploring puzzle space
                        // with these values swapped

                        // we could go back to the old (thisPuzzle) state
                        // here
                        // But in some ways it seems sensible to start
                        // searching for steps from this other state
                        // that improve our score.

                        // So, do not roll back here.
                        
                        // go back to old state
                        //inBlankMap[i] = ! inBlankMap[i];
                        //inBlankMap[j] = ! inBlankMap[j];
                        }
                    }
                else {
                    // not improvement
                    triesSinceProgress++;

                    // go back to old state
                    inBlankMap[i] = ! inBlankMap[i];
                    inBlankMap[j] = ! inBlankMap[j];

                    // we can also act on the step counter here,
                    // since we just returned to a solvable puzzle
                    if( *inTotalSteps >= inMaxTotalSteps ) {
                        return false;
                        }
                    }
                

            // mark as tried and count them
            tryMapPair[i][j] = true;
            tryMapPair[j][i] = true;
            
            tryCountPair += 2;

            
            }
        }

    
    // tried all pairs
    /*
    if( hardestScoreSeen == thisPuzzleScore ) {
        printf( "Failed to find puzzle with score harder than %d at this "
                "branch\n", hardestScoreSeen );
        }
    else {
        printf( "Hill climbing one step here improved score from %d to %d\n",
                thisPuzzleScore, hardestScoreSeen );
        }
    */
    return false;
    }	



char hillClimb( char inBlankMap[81], int *inSolution,
                RandomSource *inRandomSource,
                int inDesiredScore,
                int inMaxTotalSteps,
                int inConsecutiveFailuresBeforeGiveUp ) {

    char bestBlankMap[81];
    // start with no blanks
    int i;
for( i=0; i<81; i++ ) {
        bestBlankMap[i] = true;
        }
    int bestScore = 0;

    int totalSteps = 0;
    
    char result = hillClimbRecursive( inBlankMap, inSolution,
                                      inRandomSource,
                                      inDesiredScore,
                                      inMaxTotalSteps,
                                      inConsecutiveFailuresBeforeGiveUp,
                                      &totalSteps,
                                      bestBlankMap, &bestScore );

    // don't do anything with the best blank map for now

    return result;    
    }
