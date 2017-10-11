#include <stdio.h>
#include <limits.h>

#include "remover.h"
#include "solver.h"

#include "sudoku.h"


RandomSource *adderRandSource;


// avoid calling getRowColumnAndBlockmates in recursion
// profiler shows us spending a lot of time in that call
int allMates[81][20];
char allMatesSet = false;


/**
 * Version of add that calls itself recursively and assumes that
 * adderRandSource has been set.
 *
 * @param inClosestScore pointer to where the closest overall score seen
 *   so far is kept.
 */
int recursiveAdd( int *inPuzzle, int inLowScore, int inHighScore,
                   int *inClosestScore,
                   int *inStepRemainingCount ) {
    
    if( *inStepRemainingCount == 0 ) {
        //printf( "Hit zero\n" );
        return false;
        }
    //printf( "%d steps remaining\n", *inStepRemainingCount );
    *inStepRemainingCount = *inStepRemainingCount - 1;

    
    int *mates;
    
    
    // pick a cell that is not yet full and that we have not tried

    // flag map of cells we have already tried
    // or cells that already have values
    char tryMap[81];

    int emptyCount = 0;
    
    int i;
    for( i=0; i<81; i++ ) {
        tryMap[i] = false;

        if( inPuzzle[i] != -1 ) {
            tryMap[i] = true;
            }
        else {
            emptyCount++;
            }
        }

    
    int tryCount = 0;
    while( tryCount < emptyCount &&
           *inStepRemainingCount > 0 ) {

        i = adderRandSource->getRandomBoundedInt( 0, 80 );
    
        if( !tryMap[i] ) {
            // found a cell we haven't tried yet
            tryCount++;
            tryMap[i] = true;

            mates = allMates[i];


            char availableValueCount = 9;
                
            char valueTryMap[9];
            int v;
            for( v=0; v<9; v++ ) {
                valueTryMap[v] = false;

                int value = v+1;
                for( int m=0; m<20; m++ ) {
                    if( inPuzzle[ mates[m] ] == value ) {
                        valueTryMap[v] = true;
                        }
                    }
                
                if( valueTryMap[v] == true ) {
                    // value used by mate
                    // not available for this cell
                    availableValueCount --;
                    }
                }
            int valueTryCount = 0;

            char allValuesLeadToContradiction = true;
            
            while( valueTryCount < availableValueCount &&
                   *inStepRemainingCount > 0  ) {
                v = adderRandSource->getRandomBoundedInt( 0, 8 );

                if( !valueTryMap[v] ) {
                    // a value we haven't tried
                    valueTryMap[v] = true;
                    valueTryCount++;
                    
                    int value = v+1;

                    inPuzzle[i] = value;

                    // check if we hit a solvable puzzle
                    int *puzzleCopy = copyPuzzle( inPuzzle );
                    int score = solve( puzzleCopy );
                    delete [] puzzleCopy;
                    
                    if( score == -1 ) {
                        // still unsolvable

                        
                        
                        
                        // try adding more values
                        int result = recursiveAdd( inPuzzle,
                                                   inLowScore, inHighScore,
                                                   inClosestScore,
                                                   inStepRemainingCount );
                        
                        if( result == 1 ) {
                            // found a solution in range
                            return true;
                            }
                        else if( result == -2 ){
                            // all possible puzzles with more values filled
                            // contain a contradiction
                            }
                        else if( result == -1 ) {
                            // no in-range solution with this value set

                            // but not a contradiction
                            allValuesLeadToContradiction = false;
                            }
                        }
                    else if( score == -2 ) {
                        // contradiction with this value
                        /*
                        printf( "Hit contradition with value %d, index %d\n",
                                value, i );
                        printPuzzle( inPuzzle );
                        printf( "\n\n" );
                        */
                        }
                    else if( score >= inLowScore && score <= inHighScore ) {
                        // score in range

                        // we found a puzzle
                        return true;
                        }
                    else {
                        // solvable puzzle with out-of-range score
                        allValuesLeadToContradiction = false;
                        
                        // check if we have a new closest score

                        int oldDistance;
                        if( *inClosestScore < inLowScore ) {
                            oldDistance = inLowScore - *inClosestScore;
                            }
                        else {
                            oldDistance = *inClosestScore - inHighScore;
                            }

                        int newDistance;
                        if( *inClosestScore < inLowScore ) {
                            newDistance = inLowScore - *inClosestScore;
                            }
                        else {
                            newDistance = *inClosestScore - inHighScore;
                            }

                        if( newDistance < oldDistance ) {
                            // new closest score
                            *inClosestScore = score;
                            
                            printf( "New closest score = %d\n", score );
                            printPuzzle( inPuzzle );
                            printf( "\n\n" );
                            }
                        }
                        
                    
                    // if we got here, this value didn't work
                    // re blank value
                    inPuzzle[i] = -1;
                    }

                } // end while loop over all values for spot i

            if( availableValueCount == 0 ||
                allValuesLeadToContradiction ) {
                // found a contradiction without solver
                //  (or perhaps with solver)
                // this cell has no available values

                /*
                printf( "Hit contradition:  index %d has no possible value\n",
                        i );
                printPuzzle( inPuzzle );
                printf( "\n\n" );
                */
                
                return false;
                }
            
            }

        }  // end loop over all spots

    if( emptyCount == 0 ) {
        // a full puzzle
        return true;
        }
    
    // if we got here, then we found no good puzzle at this level of the
    // recursion
    return false;
    }



int add( int *inPuzzle, int inLowScore, int inHighScore,
         RandomSource *inRandomSource  ) {

    adderRandSource = inRandomSource;

    if( !allMatesSet ) {
        // precompute allMates
        for( int i=0; i<81; i++ ) {
            getRowColumnAndBlockmates( i, allMates[i] );
            }
        allMatesSet = true;
        }
    
    int closestScore = INT_MAX;
    int stepRemaining = 10000;
    
    int result = recursiveAdd( inPuzzle, inLowScore, inHighScore,
                               &closestScore,
                               &stepRemaining );

    if( result ) {
        printf( "Valid puzzle found\n" );
        }
    else {
        printf( "No valid puzzle found\n" );
        }

    printf( "Result:\n" );
    printPuzzle( inPuzzle );
    printf( "\n\n" );
    
    
    return result;
    }



/**
 * Recursive function for filling a grid.
 *
 * @param inPuzzle the puzzle to fill.
 *   Destroyed by caller.
 * @param inNextEmptyIndex the next cell to fill.
 *
 * @return true if inPuzzle is filled without contradiction, or false
 *   if inNextEmptyIndex cannot be filled without a contradiction.
 */
char recursiveAddToFull( int *inPuzzle, int inNextEmptyIndex ) {

    if( inNextEmptyIndex == 81 ) {
        // full puzzle
        return true;
        }
    
    int *mates = allMates[ inNextEmptyIndex ];


    // build a flag map and count of which values are available
    // (we select from these at random below)
    char availableValueCount = 9;
                
    char valueTryMap[9];
    int v;
    for( v=0; v<9; v++ ) {
        valueTryMap[v] = false;
        }

    for( int m=0; m<20; m++ ) {
        int value = inPuzzle[ mates[m] ];
        int v = value - 1;
        
        if( !valueTryMap[v] && value != -1 ) {
            valueTryMap[v] = true;

            // value used by mate
            // not available for this cell
            availableValueCount --;
            }
        }

    
    int valueTryCount = 0;
   
    while( valueTryCount < availableValueCount ) {
        v = adderRandSource->getRandomBoundedInt( 0, 8 );


        if( !valueTryMap[v] ) {
            // a value we haven't tried
            valueTryMap[v] = true;
            valueTryCount++;
    
            int value = v+1;
            
            // try this value at inNextEmptyIndex
            inPuzzle[ inNextEmptyIndex ] = value;
            if( recursiveAddToFull( inPuzzle, inNextEmptyIndex + 1 ) ) {
                // found full grid
                return true;
                }
            }
        }

    // if we got here, none of our values could fill the grid

    // back to blank
    inPuzzle[ inNextEmptyIndex ] = -1;
    return false;    
    }



void addToFull( int *inPuzzle, RandomSource *inRandomSource ) {

    adderRandSource = inRandomSource;


    if( !allMatesSet ) {
        // precompute allMates
        for( int i=0; i<81; i++ ) {
            getRowColumnAndBlockmates( i, allMates[i] );
            }
        allMatesSet = true;
        }

    
    recursiveAddToFull( inPuzzle, 0 );
    }

