
#include <stdio.h>

#include "remover.h"
#include "solver.h"

#include "sudoku.h"


#include "minorGems/util/random/RandomSource.h"



/**
 * Copy an array of flags.
 *
 * @param inCannotRemoveFlags the 81-element array to copy.
 *   Destroyed by caller.
 *
 * @return a newly allocated copy of inCannotRemoveFlags.
 *   Destroyed by caller.
 */
char *copyFlags( char *inCannotRemoveFlags ) {
    char *copy = new char[81];

    for( int f=0; f<81; f++ ) {
        copy[f] = inCannotRemoveFlags[f];
        }
    return copy;
    }



RandomSource *randSource;



/**
 * Recursively removes values from a puzzle, searching for the hardest
 * puzzle.
 *
 * @param inPuzzle the puzzle to remove values from.
 * @param inHardestScoreSeen pointer to the hardest score seen so far.
 * @param inCannotRemoveFlags 81-element array of flags.
 *   inCannotRemoveFlags[i] is true if we already know that the value in
 *   cell i cannot be removed in this puzzle.
 *
 * @return the hardest score discovered.
 */
int recursiveRemove( int *inPuzzle, int *inHardestScoreSeen,
                     char *inCannotRemoveFlags ) {

    
    int thisPuzzleScore = score( inPuzzle );
    
    if( thisPuzzleScore < 0 ) {
        // unsolvable
        return -1;
        }

    char *cannotRemoveFlags = copyFlags( inCannotRemoveFlags );
    
    /*
    printf( "This puzzle %d\n", thisPuzzleScore );
    printPuzzle( inPuzzle );
    printf( "\n" );
    */

    int hardestIndex = -1;
    int hardestScore = 0;

    int tryCount = 0;
    while( tryCount < 3 ) {

        int i = randSource->getRandomBoundedInt( 0, 80 );
    //for( int i=0; i<81; i++ ) {

        if( inPuzzle[i] != -1 &&
            ! cannotRemoveFlags[i] ) {
            // not blank and
            // we haven't yet discovered that we cannot remove it

            tryCount++;
            
            // blank it
            int oldValue = inPuzzle[ i ];
            inPuzzle[i] = -1;

            int score = recursiveRemove( inPuzzle, inHardestScoreSeen,
                                         cannotRemoveFlags );

            if( score != -1 ) {
                if( score > hardestScore ) {
                    hardestScore = score;
                    hardestIndex = i;
                    }
                }
            else {
                // cannot remove this value
                // flag it so we remember it deeper in the recursion
                cannotRemoveFlags[i] = true;
                }
            
            // restore it
            inPuzzle[i] = oldValue;
            }
        }

    delete [] cannotRemoveFlags;
    
    int returnScore;
    
    if( hardestScore > thisPuzzleScore ) {
        // found cells to remove to make it harder
        returnScore = hardestScore;
        }
    else {
        // at end of recursion branch
        returnScore = thisPuzzleScore;
        //printf( "End of branch score %d\n", thisPuzzleScore );

        if( thisPuzzleScore > *inHardestScoreSeen ) {
            score( inPuzzle, true );

            printPuzzle( inPuzzle );
            printf( "\n" );
            }
        }

    if( returnScore > *inHardestScoreSeen ) {
        *inHardestScoreSeen = returnScore;
        
        printf( "New hardest score:  %d\n", returnScore );
        }

    return returnScore;
    }



int remove( int *inPuzzle, RandomSource *inRandomSource ) {
    randSource = inRandomSource;
    
    int hardestScoreSeen = 0;

    char cannotRemoveFlags[81];

    for( int f=0; f<81; f++ ) {
        cannotRemoveFlags[f] = false;
        }

    
    int score = recursiveRemove( inPuzzle,
                                 &hardestScoreSeen, cannotRemoveFlags );

    return score;
    }



void fastRemove( int *inPuzzle, RandomSource *inRandomSource,
                 int inMaxBlank ) {
    
    // which cells we have already tried this round

    // if we failed to blank them for puzzle state A
    // then we certainly cannot blank them in state A', where A' is A with
    // additional blanks
    char tryMap[81];
    int i;
    for( i=0; i<81; i++ ) {
        

        if( inPuzzle[i] == -1 ) {
            // already blank
            // don't try it
            tryMap[i] = true;
            }
        else {
            tryMap[i] = false;
            }
        }
    

    

    char keepGoing = true;
    while( keepGoing ) {
        
        int numTryable = 0;
        int numBlank = 0;
        for( i=0; i<81; i++ ) {
            if( !tryMap[i] ) {
                numTryable++;
                }
            if( inPuzzle[i] == -1 ) {
                numBlank++;
                }
            }

        if( numBlank >= inMaxBlank ) {
            return;
            }
        
        char found = false;
        
        int numTried = 0;
        while( !found && numTried < numTryable ) {
            
            
            i = inRandomSource->getRandomBoundedInt( 0, 80 );
            
            if( !tryMap[i] ) {
                // haven't tried blanking this cell yet
                                
                // trying it now
                tryMap[i] = true;
                numTried++;

                int oldValue = inPuzzle[i];

                inPuzzle[i] = -1;

                // is it sovable?
                int puzzleScore = score( inPuzzle );
                
                if( puzzleScore > -1 ) {
                    // still solvable

                    // leave this value blank

                    found = true;
                    }
                else {
                    // not solvable

                    // check again, incase restoring j helped
                    if( puzzleScore > -1 ) {
                        found = true;
                        }
                    else {
                        // unsolvable, whether or not we restored j
                        // restore value
                        inPuzzle[i] = oldValue;
                        }
                    }
                }
            }

        // stop if we fail to find a cell to blank
        keepGoing = found;
        }
    }


    
void fastFill( int *inPuzzle, int *inSolution, RandomSource *inRandomSource ) {

    int i;

    int filledCount = 0;
    for( i=0; i<81; i++ ) {
        if( inPuzzle[i] != -1 ) {
            filledCount++;
            }
        }

    // fill with some values selected randomly from solution
    
    int numToFill = 10;
    while( filledCount < numToFill ) {
        i = inRandomSource->getRandomBoundedInt( 0, 80 );

        if( inPuzzle[i] == -1 ) {
            inPuzzle[i] = inSolution[i];
            filledCount++;
            }
        }

    
    int hardIndices[81];

    int *solverGrid = copyPuzzle( inPuzzle );

    int score = solve( solverGrid, false, hardIndices );

    while( score < 0 ) {
        // not solvable

        /*
        int numToFill = 5;
        int filledCount = 0;

        while( filledCount < numToFill ) {

            char found = false;
            while( !found ) { 
                i = inRandomSource->getRandomBoundedInt( 0, 80 );

                if( solverGrid[i] == -1 ) {
                    // blank
                    found = true;
                    }
                }
            // fill the cells from our solution
            partialGrid[i] = inPuzzle[i];

            filledCount++;
            }
        */

        // pick hardest unfilled cells and fill them
        int filledCount = 0;
        int index = 0;
        while( filledCount < 1 && index < 81 ) {
            i = hardIndices[index];

            if( solverGrid[ i ] == -1 ) { 
                // fill the cell from our solution
                inPuzzle[i] = inSolution[i];
                filledCount++;
                }
            index++;
            }
        
        // try solving again
        delete [] solverGrid;

        solverGrid = copyPuzzle( inPuzzle );

        score = solve( solverGrid, false, hardIndices );
        }

    // at this point, we have a solvable puzzle in inPuzzle

    delete [] solverGrid;
    }


