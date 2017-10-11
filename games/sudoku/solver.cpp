#include "solver.h"

#include "sudoku.h"

#include <stdio.h>

/**
 * Culls possible values in mate cells when a given index is set to a value.
 *
 * @param inIndex the cell being set.
 * @param inValue the value of the cell.
 * @param inPossibleValues the possible value map.
 *   Destroyed by caller.
 * @param inAllMates the precomputed mate map.
 *   Destroyed by caller.
 */
void cullPossibleValues( int inIndex, int inValue,
                         char inPossibleValues[81][9],
                         int inAllMates[81][20] ) {

    // only need to update mates
    
    int *mates = inAllMates[ inIndex ];

    int valueIndex = inValue - 1;
    
    for( int m=0; m<20; m++ ) {
        inPossibleValues[ mates[m] ][ valueIndex ] = false;
        }
    }


/**
 * Sets all flags to false.
 *
 * @param inValues the value flags to set.
 *   Destroyed by caller.
 */
void clearPossibleValues( char inValues[9] ) {
    // set all to false
    for( int v=0; v<9; v++ ) {
        inValues[v] = false;
        }
    }



/**
 * Gets whether a puzzle is full (no blanks).
 *
 * @param inPuzzle the puzzle.
 *   Destroyed by caller.
 *
 * @return true if full.
 */
char puzzleFull( int *inPuzzle ) {
    char foundBlank = false;
    for( int i=0; i<81 && !foundBlank; i++ ) {
        if( inPuzzle[i] == -1 ) {
            foundBlank = true;
            }
        }

    return !foundBlank;
    }



// precomputed subset sizes
// this optimization discovered with a profiler
// cuts 1/4 off execution time
char subsetsComputed = false;
int subsetSizes[512];
char subsetMaps[512][9];

void precomputeSubsets() {

    // use lowest 9 bits of vector to encode subset
    unsigned long bitVector;
    // loop over all possible 9-bit vectors (except 0)
    // thus looping over all possible non-empty subsets
    // loop from 1 to 511
    for( bitVector = 0; bitVector < 512; bitVector++ ) {
                        
        // count size
        int subsetSize = 0;

        for( int m=0; m<9; m++ ) {

            char memberInSubset = (bitVector >> m) & 0x01;

            if( memberInSubset ) {
                subsetSize++;
                subsetMaps[ bitVector ][m] = true;
                }
            else {
                subsetMaps[ bitVector ][m] = false;
                }
            }

        subsetSizes[ bitVector ] = subsetSize;
        }

    subsetsComputed = true;
    }




/**
 * Tries to make one step of progress with method A.
 *
 * @params various arrays available in the body of the solve function.
 *   Note the unconventional names (most not named "inBlah" so that code
 *   can be moved directy from the body of the solve method without vairable
 *   renaming.
 *
 * @return true if progress made.
 */
char methodA( int *inPuzzle, int allMates[81][20],
              int rowColumnAndBlockCells[3][9][9],
              int cellToGroupNumber[3][81],
              char possibleValues[81][9] ) {

    char progress = false;

    // solve method A:
    // If a number occurs as a possible value
    // only once in a row, column, or block,
    // it must be in the cell where it occurs.

        
    // stop as soon as we make progress with this method
    // thus, we will go immediately back to the simple method
    
    // for each type of group (0=row; 1=column; 2=block)
    for( int t=0; t<3 && !progress; t++ ) {

        // for each group of this type(example:  for each row)
        for( int g=0; g<9 && !progress; g++ ) {

            int *group = rowColumnAndBlockCells[t][g];

            // check if any number is possible in only one spot in
            // this group

            // for each value
            for( int v=0; v<9 && !progress; v++ ) {
                int count = 0;

                // keep track of last index we've found where
                // value is possible
                int groupIndexWhereValuePossible = -1;

                // for each member of the group
                for( int m=0; m<9; m++ ) {
                    if( possibleValues[ group[m] ][ v ] ) {
                        // this value possible for this group member
                        count ++;
                        groupIndexWhereValuePossible = m;
                        }
                    }

                if( count == 1 ) {
                    progress = true;

                    int value = v+1;
                        
                    int cellIndex =
                        group[ groupIndexWhereValuePossible ];
                        
                    clearPossibleValues( possibleValues[ cellIndex ] );

                    inPuzzle[ cellIndex ] = value;

                    cullPossibleValues( cellIndex, value,
                                        possibleValues,
                                        allMates );
                    }
                } // end loop over values
            } // end loop over groups
        } // end loop over types of groups

    return progress;
    }



/**
 * See description above for methodA.
 */
char methodB( int *inPuzzle, int allMates[81][20],
              int rowColumnAndBlockCells[3][9][9],
              int cellToGroupNumber[3][81],
              char possibleValues[81][9] ) {

    char progress = false;
    
    // solve method B:
    // If a number in a row occurs only in one block, eliminate
    // that number from other cells in the block

    // Do same for columns vs. blocks.

    // Also do for blocks vs. rows and
    //             blocks vs. columns
    
    // stop as soon as we make progress with this method
    // thus, we will go immediately back to the simple method
    
            
    // for each type of group (0=row; 1=column; 2=block )
    for( int t1=0; t1<3 && !progress; t1++ ) {

        // for each type of intersecting group
        // (0=row; 1=column; 2=block )
        for( int t2=0; t2<3 && !progress; t2++ ) {

            // skip cases that are not interesting
            if(
                // rows/columns vs blocks
                ( t1 == 0 || t1 == 1 ) && t2 == 2  
                ||  // or
                // blocks vs rows/columns
                t1 == 2 && ( t2 == 0 || t2 == 1 )
                // skip blocks vs. blocks, rows vs. columns, etc
            ) {
                    
                // for each group of type t1 (example:  for each row)
                for( int g=0; g<9 && !progress; g++ ) {

                    int *group = rowColumnAndBlockCells[t1][g];

                    // for each value
                    for( int v=0; v<9 && !progress; v++ ) {
                        // count how many intersecting groups this
                        // value occurs in
                        int count = 0;
                        
                        char intersectingGroupHitFlags[9];
                        int w;
                        for( w=0; w<9; w++ ) {
                            intersectingGroupHitFlags[w] = false;
                            }

                        
                        // keep track of last index we've found
                        // in intersecting group where
                        // value is possible
                        int lastIntersectingGroupIndex = -1;

                        // for each member of the group
                        for( int m=0; m<9; m++ ) {
                            int memberIndex = group[m];
                            if( possibleValues[ memberIndex ][ v ] ) {
                                // this value possible for this
                                // group member

                                // what intersecting group
                                // is this member in?
                                int w =
                                    cellToGroupNumber
                                    [t2]
                                    [ memberIndex ];

                                if( ! intersectingGroupHitFlags[w] ) {
                                    // an intersecting group
                                    // that was not hit before

                                    // count it
                                    count++;

                                    // flag it
                                    intersectingGroupHitFlags[w] =
                                        true;

                                    // remember it
                                    lastIntersectingGroupIndex = w;
                                    }
                                }
                            }

                        if( count == 1 ) {
                            // in this group, value v occurs only in
                            // one intersecting group

                            // remove v as a possible value from
                            // other cells in the intersecting group

                            int intersectingGroup =
                                lastIntersectingGroupIndex;

                                    
                                    
                            // for each cell in intersecting group
                            for( int w=0; w<9; w++ ) {
                                int index =
                                    rowColumnAndBlockCells
                                    [t2][ intersectingGroup ][w];

                                if( cellToGroupNumber[t1][ index ]
                                    == g ) {
                                    // this intersecting group cell is
                                    // in our group
                                    // leave possible value alone
                                    }
                                else {
                                    // in the intersecting group but
                                    // not in our group

                                    // only progress if we actually
                                    // remove a value that we used
                                    // to have marked as possible
                                    if( possibleValues[ index ][v] ) {
                                        // remove v as a possible value
                                            
                                        possibleValues[ index ][v] =
                                            false;
                                                
                                        progress = true;
                                        }
                                    }
                                    
                                }

                            }
                        }
                    }  // end loop over groups of type t1
                }  // end if statement to skip uninteresting cases
            } // end loop over intersecting group types (t2)
        } // end loop over main group types (t1)

    return progress;
    }



/**
 * See description above for methodA.
 */
char methodC( int *inPuzzle, int allMates[81][20],
              int rowColumnAndBlockCells[3][9][9],
              int cellToGroupNumber[3][81],
              char possibleValues[81][9] ) {

    char progress = false;
    
    // solve method C

    // if N numbers each occur in N cells of a group, and
    // all numbers are in the same cells, then those numbers must
    // be in those cells, so other options in the cells can be
    // eliminated

    // for eample, if one cell has values 2357
    // and another cell in the group has values 1345
    // and no other cells in the group have values 3 or 5,
    // then we can eliminate the other values in these cells, leaving
    // the two cells both marked with 35
    
            
    // stop as soon as we make progress with this method
    // thus, we will go immediately back to the simple method

    
    // easiest implementation:
    // examine all subsets of the group
    // If subset of size N contains value v N times, and v does
    // not occur outside the subset, count it.
    // if we count N such values, then we can eliminate other values
    // in our subset

    // for each type of group (0=row; 1=column; 2=block)
    for( int t=0; t<3 && !progress; t++ ) {

        // for each group of this type (example:  for each row)
        for( int g=0; g<9 && !progress; g++ ) {

            int *group = rowColumnAndBlockCells[t][g];


            // use lowest 9 bits of vector to encode subset
            unsigned long bitVector;
            // loop over all possible 9-bit vectors (except 0)
            // thus looping over all possible non-empty subsets
            // loop from 1 to 511
            for( bitVector = 1;
                 bitVector < 512 && !progress;
                 bitVector++ ) {

                // use precomputed size and map
                int subsetSize = subsetSizes[ bitVector ]; 
                // subsetMap[m] true if group member m is in subset
                char *subsetMap = subsetMaps[ bitVector ];
                
                // rule out subsets that are too big or too small
                if( subsetSize > 1 && subsetSize < 5 ) {
                    
                // flags for values that are possible
                // only in our subset
                // and that are possible at ever spot in our
                // subset
                char candidateValues[9];
                int numCandidates = 0;

                // make sure subset members are all blank
                char subsetAllBlank = true;
                        
                for( int m=0; m<9 && subsetAllBlank; m++ ) {

                    if( subsetMap[m] ) {
                        if( inPuzzle[ group[m] ] != -1 ) {
                            subsetAllBlank = false;
                            }
                        }
                    }

                // wow
                // This simple check (ruling out subsets that
                // obviously won't work) cut program time (for
                // entire program, including many calls to solve)
                // in half
                if( subsetAllBlank ) {
                        
                    // for each value
                    for( int v=0; v<9; v++ ) {
                        
                        char valueInEverySubsetMember = true;
                        char valueOnlyInSubset = true;

                        // keepGoing =
                        // valueOnlyInSubset && valueInEverySubsetMember
                        char keepGoing = true;
                            
                        // for each member of group
                        for( int m=0; m!=9 && keepGoing; m++ ) {

                            char memberInSubset = subsetMap[m];

                            int memberIndex = group[m];

                            char valuePossible =
                                possibleValues[ memberIndex ][v];
                                
                            if( memberInSubset ) {
                                if( !valuePossible ) {
                                    valueInEverySubsetMember = false;
                                    keepGoing = false;
                                    }
                                }
                            else {
                                if( valuePossible ) {
                                    valueOnlyInSubset = false;
                                    keepGoing = false;
                                    }
                                }
                            }

                        if( valueOnlyInSubset &&
                            valueInEverySubsetMember ) {
                            candidateValues[v] = true;
                            numCandidates++;
                            }
                        else {
                            candidateValues[v] = false;
                            }
                        }

                    // now we have all candidates for this subset

                    if( numCandidates == subsetSize ) {
                        // a hit

                        // eliminate other values from our subset

                        /*
                          printf( "Subset " );
                          for( int m=0; m<9; m++ ) {
                          if( subsetMap[m] ) {
                          printf( "1" );
                          }
                          else {
                          printf( "0" );
                          }
                          }
                          printf( " of group (type %d) %d contains only "
                          "values ", t, g );
                          for( int v=0; v<9; v++ ) {

                          if( candidateValues[v] ) {
                          printf( "%d", v+1 );
                          }
                          }
                          printf( "\n" );
                        */
                            
                        for( int m=0; m<9; m++ ) {

                            if( subsetMap[m] ) {
                                // in subset

                                int i = group[m];

                                for( int v=0; v<9; v++ ) {

                                    if( !candidateValues[v] ) {

                                        // not one of our values

                                        if( possibleValues[i][v] ) {
                                            // mark as not possible

                                            possibleValues[i][v] = false;
                                            progress = true;
                                            /*
                                              printf( "Removing %d from "
                                              "cell %d\n", v+1, i );
                                            */
                                            }
                                        }
                                    }
                                }
                            }
                            
                        } // end check for hit

                    } // end check for subset that is all blank

                    } // end check to rule out too-small or too-large subsets  
                        
                } // end loop over all subsets
            } // end loop over groups
        } // end loop over group types

    return progress;
    }



/**
 * See description above for methodA.
 *
 * Difference:
 * @return the size of the subset discovered, or 0 if no progress was made.
 */
int methodD( int *inPuzzle, int allMates[81][20],
             int rowColumnAndBlockCells[3][9][9],
             int cellToGroupNumber[3][81],
             char possibleValues[81][9] ) {

    char progress = false;
    
    // solve method D

    // if N cells of a group collectively contain a total of N numbers,
    // then those numbers must occur in those N cells.
    // Thus, they can be eliminated as possibilities from the other cells
    // in the group

    // for eample, if one cell has values 235, and another cell has the
    // values 23, and a third cell has the value 35, then collectively
    // these three cells must have the values 235.  Thus, we can eliminate
    // these values from other cells in the group.
            
    // stop as soon as we make progress with this method
    // thus, we will go immediately back to the simple method

    
    // easiest implementation:
    // examine all subsets of the group
    // Look for subset of size N that contains, in total, N possible values 

    // for each type of group (0=row; 1=column; 2=block)
    for( int t=0; t<3 && !progress; t++ ) {

        // for each group of this type (example:  for each row)
        for( int g=0; g<9 && !progress; g++ ) {

            int *group = rowColumnAndBlockCells[t][g];


            // use lowest 9 bits of vector to encode subset
            unsigned long bitVector;
            // loop over all possible 9-bit vectors (except 0)
            // thus looping over all possible non-empty subsets
            // loop from 1 to 511
            for( bitVector = 1;
                 bitVector < 512 && !progress;
                 bitVector++ ) {

                // use precomputed size and map
                int subsetSize = subsetSizes[ bitVector ]; 
                // subsetMap[m] true if group member m is in subset
                char *subsetMap = subsetMaps[ bitVector ];
                
                // rule out subsets that are too big or too small
                if( subsetSize > 1 && subsetSize < 5 ) {

                // make sure subset members all blank
                char subsetAllBlank = true;
                        
                for( int m=0; m<9 && subsetAllBlank; m++ ) {

                    if( subsetMap[m] ) {
                        if( inPuzzle[ group[m] ] != -1 ) {
                            subsetAllBlank = false;
                            }
                        }
                    }

                // wow
                // This simple check (ruling out subsets that
                // obviously won't work) cut program time (for
                // entire program, including many calls to solve)
                // in half
                if( subsetAllBlank ) {

                    // track possible values for group using a flag array
                    char netPossibleValues[9] =
                        { false, false, false, false,
                          false, false, false, false,
                          false };

                    int possibleValueCount = 0;
                    
                    // for each member of group
                    for( int m=0; m!=9; m++ ) {
                        
                            
                        if( subsetMap[m] ) {
                            // member in subset

                            // index of member in puzzle
                            int memberIndex = group[m];
                                
                            // for each value
                            for( int v=0; v<9; v++ ) {

                                if( possibleValues[ memberIndex ][v] ) {
                                    if( !netPossibleValues[v] ) {
                                        // value not yet flagged

                                        // count it
                                        possibleValueCount++;

                                        // flag it
                                        netPossibleValues[v] = true;
                                        }
                                    }
                                }
                            }
                        }


                    if( possibleValueCount == subsetSize ) {
                        // a hit

                        // cull values from other cells in group
                        for( int m=0; m<9; m++ ) {

                            if( !subsetMap[m] ) {
                                // a member that is not in subset

                                int memberIndex = group[m];

                                for( int v=0; v<9; v++ ) {
                                    if( netPossibleValues[v] ) {
                                        if( possibleValues[memberIndex][v] ) {
                                            // this value marked as possible

                                            // thus, we have made progress
                                            progress = true;
                                            
                                            possibleValues[memberIndex][v] =
                                                false;
                                            }
                                        }
                                    }
                                }
                            }

                        if( progress ) {
                            return subsetSize;
                            }
                        }

                    } // end check for subset that is all blank

                    } // end check to rule out too-small or too-large subsets  
                        
                } // end loop over all subsets
            } // end loop over groups
        } // end loop over group types

    // did not find one
    return 0;
    }



int solve( int *inPuzzle, char inPrintReport,
           int *outHardIndices, char inStopOnProgress ) {

    // precompute subset sizes if necessary
    if( !subsetsComputed ) {
        precomputeSubsets();
        }
    
    // count how many times each method was used
    // we use these to compute the hardness
    int countBasic = 0;
    int countA = 0;
    int countB = 0;
    int countC = 0;

    // count how many times method D succeeded, and track subset size of
    // success
    int countD[10] = {0,0,0,0,0,0,0,0,0,0};

    
    // allMates[i] is an array of row, column, and blockmates indices
    // for cell i
    int allMates[81][20];

    // precompute allMates
    int i;
    for( i=0; i<81; i++ ) {
        getRowColumnAndBlockmates( i, allMates[i] );
        }


    // store in a 3d array so we can loop over each set
    int rowColumnAndBlockCells[3][9][9];
    
    // precomputed indices of cells in each row
    // rowCells[r] is array of cell indices in row r
    int *rowCells[9];
    // columnCells[c] is array of cell indices in column c
    int *columnCells[9];
    // blockCells[b] is array of cell indices in block b
    int *blockCells[9];

    // maps each cell to a particular group
    // for example, which row is cell i in?
    // cellToGroupNumber[0][i] tells which row number cell i is in.
    // cellToGroupNumber[1][i] tells which column number cell i is in.
    // cellToGroupNumber[2][i] tells which block number cell i is in.
    
    int cellToGroupNumber[3][81];

    for( int g=0; g<9; g++ ) {
        rowCells[g] = rowColumnAndBlockCells[0][g];
        columnCells[g] = rowColumnAndBlockCells[1][g];
        blockCells[g] = rowColumnAndBlockCells[2][g];
        
        for( int h=0; h<9; h++ ) {

            // cell h in row g
            rowCells[g][h] = 9 * g + h;

            // reverse map
            cellToGroupNumber[0][ rowCells[g][h] ] = g;

            
            // cell h in column g
            columnCells[g][h] = 9 * h + g;

            // reverse map
            cellToGroupNumber[1][ columnCells[g][h] ] = g;
            
            
            blockCells[g][h] =
                // necessary to get us to first cell in block
                9 * (g / 3) * 3
                + 3 * ( g - ( g / 3 ) * 3 )
                // now add more to get into first cell of correct row of block
                + 9 * ( h / 3 )
                // now add more to get into correct column
                + ( h - ( h / 3 ) * 3 );
            
            // reverse map
            cellToGroupNumber[2][ blockCells[g][h] ] = g;

            }
        }
    /*
    printf( "Cell to row mapping:\n" );
    printPuzzle( cellToGroupNumber[0] );
    printf( "\nCell to column mapping:\n" );
    printPuzzle( cellToGroupNumber[1] );
    printf( "\nCell to block mapping:\n" );
    printPuzzle( cellToGroupNumber[2] );

    printf( "\n" );
    */
    
    // possibleValues[i][v-1] is true if v is a possible value for cell i

    // possibleValues[i] is false for all v if inPuzzle[i] is set to a value
    // thus, possibleValues for set cells do not interfere with our solving
    // methods
    char possibleValues[81][9];

    // all cells start out with all possible values
    for( i=0; i<81; i++ ) {
        for( int v=0; v<9; v++ ) {
            possibleValues[i][v] = true;
            }
        }

    // use givens to cull possible values
    for( i=0; i<81; i++ ) {
        int value = inPuzzle[i]; 
        if( value > 0 ) {

            // clean up possible values for this cell
            clearPossibleValues( possibleValues[i] );

            // cull from mate cells
            cullPossibleValues( i, value, possibleValues, allMates );
            }
        }


    

    // main solver loop
    
    char progress = true;

    
    char stop = false;
    
    while( progress && !stop ) {
        progress = false;


        //printf( "Trying basic solve mode\n" );

        // basic solve mode:
        // look for cells that have only one possible value and set them
        // cull values from mate cells as values are set
        // repeat
        
        progress = true;
        
        while( progress && !stop ) {
            progress = false;

            // look at each still-blank cell
            for( i=0; i<81 && !stop; i++ ) {
                if( inPuzzle[i] == -1 ) {

                    int numValues = countPossibleValues( possibleValues[i] );
                    
                    if( numValues == 1 ) {

                        // one possible value for this blank cell

                        int value = getFirstPossibleValue( possibleValues[i] );
                        
                        // set it
                        inPuzzle[i] = value;

                        // set cells are marked with no possible values
                        clearPossibleValues( possibleValues[i] );
                        
                        // cull value from mates
                        cullPossibleValues( i, value,
                                            possibleValues, allMates );

                        progress = true;
                        if( inStopOnProgress ) {
                            stop = true;
                            }

                        countBasic++;
                        }
                    else if( numValues == 0 ) {
                        // no values left for this cell

                        // contradiction
                        return -2;
                        }
                    

                    }
                }
            }

        
        // at this point, we have stopped making progress with basic mode
        

        if( !stop && !puzzleFull( inPuzzle ) ) {
            //printf( "Basic solve mode stuck, trying solve mode A\n" );

            
            progress = methodA( inPuzzle, allMates,
                                rowColumnAndBlockCells,
                                cellToGroupNumber,
                                possibleValues );
            
            if( progress ) {
                countA++;

                // method A only makes progress if it sets some new
                // cell's value, so it counts as one step                
                if( inStopOnProgress ) {
                    stop = true;
                    }
                }
            } // end check if puzzle full



        

        if( !stop && !progress && !puzzleFull( inPuzzle ) ) {

            /*
            printf( "Applying solve method B to this state:\n" );
            printPuzzle( inPuzzle );
            printf( "\n" );
            */
            
            progress = methodB( inPuzzle, allMates,
                                rowColumnAndBlockCells,
                                cellToGroupNumber,
                                possibleValues );
            
            if( progress ) {
                countB++;

                // don't check inStopOnProgress on progress here
                // because method B doesn't actually set a new cell's value
                // (progress with method B just culls some possible values)
                }
            
            } // end check for full puzzle
        




        if( !stop && !progress && !puzzleFull( inPuzzle ) ) {

            /*
            printf( "Applying solve method C to this state:\n" );
            printPuzzle( inPuzzle );
            printf( "\n" );
            */
            
            progress = methodC( inPuzzle, allMates,
                                rowColumnAndBlockCells,
                                cellToGroupNumber,
                                possibleValues );
            
            if( progress ) {
                countC++;

                // don't check inStopOnProgress on progress here
                // because method C doesn't actually set a new cell's value
                // (progress with method C just culls some possible values)
                }

            } // end method C



        if( !stop && !progress && !puzzleFull( inPuzzle ) ) {

            /*
            printf( "Applying solve method D to this state:\n" );
            printPuzzle( inPuzzle );
            printf( "\n" );
            */
            
            int subsetSize = methodD( inPuzzle, allMates,
                                      rowColumnAndBlockCells,
                                      cellToGroupNumber,
                                      possibleValues );
            
            if( subsetSize > 0 ) {
                countD[ subsetSize ] ++;
                progress = true;

                // don't check inStopOnProgress on progress here
                // because method D doesn't actually set a new cell's value
                // (progress with method D just culls some possible values)
                }
            

            } // end method D

        
        
        
            }

    if( inPrintReport ) {
        printf( "Basic method count:  %d\n", countBasic );
        printf( "Method A count:  %d\n", countA );
        printf( "Method B count:  %d\n", countB );
        printf( "Method C count:  %d\n", countC );
        printf( "Method D count:  " );
        for( int s=0; s<10; s++ ) {
            printf( "%d {%d}  ", countD[s], s );
            }
        printf( "\n" );
        }
    
    if( puzzleFull( inPuzzle ) ) {

        int dScore = 0;
        for( int s=0; s<10; s++ ) {
            // 32 points if subset of size 2 found
            // 48 points if subset of size 3 found
            // 64 points if subset of size 4 found
            // etc
            dScore += countD[s] * 16 * s;
            }
        return
            countBasic +
            4 * countA +
            8 * countB +
            16 * countC +
            dScore;
        }
    else {

        // solving failed

        if( outHardIndices != NULL ) {

            // first, count values for each cell
            int valueCounts[81];
            for( i=0; i<81; i++ ) {
                valueCounts[i] = countPossibleValues( possibleValues[i] );
                }

            // now fill in outHardIndices
            int index = 0;
            // for each possible value count, starting with largest count
            for( int c=9; c>=0; c-- ) {
            //for( int c=0; c<=9; c++ ) {

                for( i=0; i<81; i++ ) {
                    if( valueCounts[i] == c ) {
                        outHardIndices[index] = i;
                        index++;
                        }
                    }
                }            
            }
        
        return -1;
        }
    }



int score( int *inPuzzle, char inPrintReport ) {

    int *puzzleCopy = copyPuzzle( inPuzzle );
    int returnValue = solve( puzzleCopy, inPrintReport );

    delete [] puzzleCopy;

    return returnValue;
    }

