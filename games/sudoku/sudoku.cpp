/*
 * Modification History
 *
 * 2006-March-30   Jason Rohrer
 * Turned checking for other valid places for a value into a function.
 * Improved findValidPlacesFor function.  Still more work to do.
 *
 * 2006-April-14   Jason Rohrer
 * Got restarting hill climbing in place.
 * Major code clean up.
 *
 * 2006-April-24   Jason Rohrer
 * Fixed fill-in post-processing so that it makes a balanced puzzle.
 * Fixed fill-in post-processing to prevent it from dipping too low.
 */


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "minorGems/util/random/StdRandomSource.h"
#include "minorGems/io/file/File.h"
#include "minorGems/util/stringUtils.h"
#include "minorGems/util/SimpleVector.h"
#include "minorGems/system/Time.h"


#include "sudoku.h"
#include "solver.h"
#include "remover.h"
#include "adder.h"
#include "hillClimb.h"


/*

  To-do:

  x- Transform template
     x- rename symbols
     x- swap rows and columns within blocks
     x- swap blocks of rows and columns

  x- Figure out how to format as EPS (maybe use a template and populate
     it with a perl script)

  x- Improve constrained cell detection more.
  -- Try guess and check when a cell has more than one possible value
     (hard puzzles only)
     Got this working (checkForConflict), but still needs tweaking
     
 */

// the score to shoot for.
// can be overridden on command line
int targetScore = 30;

const char *templateFileName = "grid_template.eps";


// make true to show steps as cells are turned blank
char showSteps = false;



int templatePuzzle[81] = { 1, 2, 3, 7, 8, 4, 5, 6, 9,
                           4, 5, 6, 1, 2, 9, 8, 7, 3,
                           7, 8, 9, 3, 6, 5, 1, 2, 4,
                           3, 1, 5, 2, 4, 7, 6, 9, 8,
                           2, 9, 8, 6, 1, 3, 7, 4, 5,
                           6, 4, 7, 5, 9, 8, 3, 1, 2,
                           5, 3, 1, 4, 7, 2, 9, 8, 6,
                           8, 6, 2, 9, 5, 1, 4, 3, 7,
                           9, 7, 4, 8, 3, 6, 2, 5, 1 };
/*
int templatePuzzle[81] = { 5,6,4,9,1,8,3,2,7,
                           3,1,2,4,7,6,5,8,9,
                           9,8,7,2,3,5,1,6,4,
                           6,4,5,1,8,9,2,7,3,
                           1,7,8,3,4,2,6,9,5,
                           2,3,9,6,5,7,4,1,8,
                           7,9,1,5,6,3,8,4,2,
                           8,5,6,7,2,4,9,3,1,
                           4,2,3,8,9,1,7,5,6 };
*/


int evilPuzzle[81] = { -1,-1,-1, -1,-1, 8, -1,-1, 7,
                        3,-1,-1, -1,-1,-1, -1, 8, 9,
                        9,-1,-1,  2, 3,-1, -1,-1,-1,

                       -1, 4,-1,  1,-1,-1,  2,-1,-1,
                       -1,-1, 8, -1, 4,-1,  6,-1,-1,
                       -1,-1, 9, -1,-1, 7, -1, 1,-1,
                       
                       -1,-1,-1, -1, 6, 3, -1,-1, 2,
                        8, 5,-1, -1,-1,-1, -1,-1, 1,
                        4,-1,-1,  8,-1,-1, -1,-1,-1 };
                       
// puzzles are stored in row-major order in an 81-element array








int *copyPuzzle( int *inPuzzle ) {
    int *copy = new int[81];
    
    for( int i=0; i<81; i++ ) {
        copy[i] = inPuzzle[i];
        }
    return copy;
    }



/**
 * Prints a valid map to standard out.
 *
 * @param inMap the map to print.  Destroyed by caller.
 */
void printValidMap( char *inMap );


/**
 * Checks if a puzzle is valid (satisfies constraints).
 *
 * @param inPuzzle the puzzle to check.  Destroyed by caller.
 *
 * @return true if the puzzle is correct.
 */
char checkPuzzle( int *inPuzzle );



/**
 * Attempts to solve a puzzle by filling in as many blanks as possible
 * using logic only.
 *
 * @param inPuzzle the puzzle to solve.  Destroyed by caller.
 * @param inSolveLevel the sophistication level of the solver.
 *   Defaults to 0 (most basic).
 * @param inStopWhenIndexSet set solver to stop when this index in the
 *   puzzle gets a value set.  Defaults to -1 (keep going until entire
 *   puzzle full or we fail to solve puzzle).
 *
 * @return true if solved completely
 *   (or if value found for inStopWhenIndexSet).
 */
char solvePuzzle( int *inPuzzle, int inSolveLevel = 0,
                  int inStopWhenIndexSet = -1 );



/**
 * Attempts to solve a puzzle by filling in as many blanks as possible
 * using logic only.
 *
 * Recurses on cells related to a specific cell.
 *
 * @param inPuzzle the puzzle to solve.  Destroyed by caller.
 * @param inIndex the index to focus on.
 * @param inSolveLevel the sophistication level of the solver.
 *   Defaults to 0 (most basic).
 *
 * @return true if solved completely
 *   (or if value found for inStopWhenIndexSet).
 */
char solvePuzzleForIndex( int *inPuzzle, int inIndex, int inSolveLevel = 0 );



/**
 * Finds valid possible locations for a given value in a partially-completed
 * puzzle.
 *
 * @param inValue the value to find places for.
 * @param inPartialPuzzle the partially-filled puzzle (-1 in empty spots).
 *   Destroyed by caller.
 * @param outValidMap map that will be filled in with true/false values
 *   showing valid positions for inValue.  Map must be allocated as an 81-char
 *   array by caller.
 *   Destroyed by caller.
 *
 * @return true if there are any valid positions left for inValue.
 */
char findValidPlacesFor( int inValue, int *inPartialPuzzle, 
                         char *outValidMap );



/**
 * Checks if inIndex is the only valid location in one of its row, column,
 * or block.
 *
 * @param inIndex the index of the cell to check.
 * @param inValidMap the valid map (81-element true/false array) to check.
 *   Destroyed by caller.
 *
 * @return true if inIndex is the only valid cell in at least one of its
 *   groups.
 */
char isOnlyValid( int inIndex, char *inValidMap );



/**
 * Removes conflicts from a valid map.
 *
 * @param inValidMap the valid map (81-element true/false array) to fix.
 *   Destroyed by caller.
 */
void removeConflictsFromValidMap( char *inValidMap,
                                  char inUseAdvanced = false );


/**
 * Removes conflicts in a valid map stemming from all valids in one
 * group (for example, a row) occurring in another group (for example, a
 * colunm).  In these situations, the other valids in the the second group
 * are removed.
 *
 * @param inValidMap the valid map (81-element true/false array) to fix.
 *   Destroyed by caller.
 * @param inMateSetA the first mate set (8-element array of cell indices).
 *   Destroyed by caller.
 * @param inMateSetB the second mate set.  This is the set that will
 *   potentially have valids removed from it.
 *   Destroyed by caller.
 *
 * @return true if this call resulted in a change to the valid map.
 */
char removeOverlappingMateConflicts( char *inValidMap, int *inMateSetA,
                                     int *inMateSetB );



/**
 * Counts how many values are allowed in a given blank space.
 *
 * @param inIndex the index of the cell to check.
 * @param inPartialPuzzle the partially-filled puzzle (-1 in empty spots).
 *   Destroyed by caller.
 *
 * @return the number of possible values for the cell.
 */
int countPossibleValues( int inIndex, int *inPartialPuzzle );



/**
 * Gets the values that are allowed in a given space.
 *
 * @param inIndex the index of the cell to check.
 * @param inPartialPuzzle the partially-filled puzzle (-1 in empty spots).
 *   Destroyed by caller.
 *
 * @return a vector of possible values.
 *   Destroyed by caller.
 */
SimpleVector<long> *getPossibleValues( int inIndex, int *inPartialPuzzle ); 



/**
 * Checks a partially-filled puzzle for conflicts that would make it
 * unsolvable.
 *
 * @param inPartialPuzzle the partially-filled puzzle (-1 in empty spots).
 *   Destroyed by caller.
 *
 * @param inRecursionLimit pointer to the limit on the number of recursive
 *   steps explored.  This function returns false immediately if
 *   inRecursionLimit is set to 0.
 *
 * @return true if a conflict is found.
 */
char checkForConflict( int *inPartialPuzzle, int *inRecursionLimit );






/**
 * Rename all sybols randomly.
 *
 * @param inPuzzle the puzzle to rename symbols in.  Destroyed by caller.
 * @param inRandSource the random source to use.  Destroyed by caller.
 */
void randomRename( int *inPuzzle, RandomSource *inRandSource );



/**
 * Swap two rows in a puzzle.
 *
 * @param inPuzzle the puzzle to swap rows in.  Destroyed by caller.
 * @param inRowA the first row.
 * @param inRowB the second row.
 */
void swapRows( int *inPuzzle, int inRowA, int inRowB ); 



/**
 * Swap two columns in a puzzle.
 *
 * @param inPuzzle the puzzle to swap columns in.  Destroyed by caller.
 * @param inColumnA the first column.
 * @param inColumnB the second column.
 */
void swapColumns( int *inPuzzle, int inColumnA, int inColumnB ); 



/**
 * Swap two row blocks in a puzzle.
 *
 * @param inPuzzle the puzzle to swap rows in.  Destroyed by caller.
 * @param inRowBlockA the first row block (in range [0..2].
 * @param inRowBlockB the second row block (in range [0..2].
 */
void swapRowBlocks( int *inPuzzle, int inRowBlockA, int inRowBlockB ); 



/**
 * Swap two column blocks in a puzzle.
 *
 * @param inPuzzle the puzzle to swap columns in.  Destroyed by caller.
 * @param inColumnBlockA the first column block (in range [0..2].
 * @param inColumnBlockB the second column block (in range [0..2].
 */
void swapColumnBlocks( int *inPuzzle, int inColumnBlockA, int inColumnBlockB );




// returns score of generated puzzle
int main( int inNumArgs, char **inArgs ) {
    
    StdRandomSource *randSource = new StdRandomSource();

    int puzzle[81];
    int solution[81];
    
    
    int startingSeed = 1;

    char onlyGenerateOne = false;
    if( inNumArgs > 1 ) {
        // first arg is starting seed

        sscanf( inArgs[1], "%d", &startingSeed );

        onlyGenerateOne = true;
        }


    if( inNumArgs > 2 ) {
        // second arg is score to achieve

        sscanf( inArgs[2], "%d", &targetScore );
        }
    


    
    int puzzleSeed = startingSeed;

    char generateMultiplePuzzles = true;
    
    
    while( generateMultiplePuzzles ) {

        if( onlyGenerateOne ) {
            // only generate 1
            generateMultiplePuzzles = false;
            }
        
        delete randSource;
        randSource = new StdRandomSource( puzzleSeed );
        
        // hill climbing

        int hardestScoreSeen = 0;
        int timeFactor = 0;
        int puzzleScore = 0;
        
        while( hardestScoreSeen < targetScore ) {
            // amount of time used increases during each round
            timeFactor++;
            
            printf( "    timeFactor = %d\n", timeFactor );

            for( int t=0;
                 t<10 && hardestScoreSeen < targetScore;
                 t++ ) {
                
                printf( "Try %d\n", t );
                // generate start point

                // generate a solution

                // start blank
                int i;
                for( i=0; i<81; i++ ) {
                    solution[i] = -1;
                    puzzle[i] = -1;
                    }

                // fill to make solution
                addToFull( solution, randSource );
                
                // get a puzzle to start from
                fastFill( puzzle, solution, randSource );
                
                // blankFlags[i] true if cell i full
                char blankFlags[81];
                int trueCount = 0;
                
                for( i=0; i<81; i++ ) {
                    if( puzzle[i] == -1 ) {
                        blankFlags[i] = 0;
                        }
                    else {
                        blankFlags[i] = 1;
                        trueCount++;
                        }
                    }
                
                // hill climb from this start point
                hillClimb( blankFlags, solution, randSource, targetScore,
                           // always at least 50 steps
                           100 * timeFactor + 50,  
                           50 );  // give up at a branch after 50 consecutive
                                  // failed tries

                // turn the blank map into the resulting puzzle

                for( i=0; i<81; i++ ) {
                    if( blankFlags[i] ) {
                        puzzle[i] = solution[i];
                        }
                    else {
                        puzzle[i] = -1;
                        }
                    }

                puzzleScore = score( puzzle );
        
                if( puzzleScore > hardestScoreSeen ) {
                    hardestScoreSeen = puzzleScore;
                    printf( "New hardest score = %d\n", hardestScoreSeen );
                    }
                else {
                    printf( "    saw score = %d\n", puzzleScore );
                    }
            
                }
            }

        

        int tryCount = 0;
        
        while( puzzleScore > targetScore && tryCount < 500 ) {
            printf( "Score too high (%d), filling one blank\n", puzzleScore );

            // pick a random blank cell
            int randomCell = randSource->getRandomBoundedInt( 0, 80 );

            while( puzzle[ randomCell ] != -1 ) {
                randomCell = randSource->getRandomBoundedInt( 0, 80 );
                }

            // fill in value from solution
            puzzle[ randomCell ] = solution[ randomCell ];


            int oldScore = puzzleScore;
            
            // compute new score
            puzzleScore = score( puzzle );

            if( puzzleScore < targetScore - 5 ) {
                printf( "We have gone too far below target score (%d), "
                        "backing up.\n", puzzleScore );

                puzzle[ randomCell ] = -1;
                puzzleScore = oldScore;
                }
            
            tryCount++;
            }

        if( puzzleScore > targetScore ) {
            printf( "Exhausted try limit for reducing score (%d) by filling "
                    "in blanks\n", puzzleScore );
            }

        
        printf( "Trying to solve puzzle with logic only\n" );
        int *puzzleCopy = copyPuzzle( puzzle );
        puzzleScore = solve( puzzleCopy, true );

        delete [] puzzleCopy;

        if( puzzleScore != -1) {
            printf( "Solved, score = %d\n", puzzleScore );
            }
        else {
            printf( "NOT Solved\n" );
            }


        // Old classification method
        // easy: 35--37 hints
        // medium: 31 hints
        // hard:   28 hints
        // very hard:  26 hints  (how is this possible?)
        
        if( puzzleScore != -1 ) {
            printf( "Puzzle (seed=%d):\n\n", puzzleSeed );
            printPuzzle( puzzle );
            printf( "\n" );
            printf( "Compact: " );
            printCompactPuzzle( puzzle );
            printf( "\n\n" );

            char solutionValid = false;
            
            if( checkPuzzle( solution ) ) {
                printf( "Valid " );
                solutionValid = true;
                }
            else {
                printf( "INVALID " );
                }
        
            printf( "Solution:\n" );
            printPuzzle( solution );
            printf( "\n\n" );

            if( solutionValid ) {
                char puzzleFileName[99];
                char solutionFileName[99];

                char puzzleLayoutFileName[99];

                char puzzleNumberString[99];
                char puzzleScoreString[99];
                
                sprintf( puzzleFileName, "%d_puzzle_%d.eps",
                         puzzleSeed, puzzleScore );
                sprintf( puzzleLayoutFileName, "%d_puzzle_%d_layout.tex",
                         puzzleSeed, puzzleScore );
                sprintf( solutionFileName, "%d_solution.eps", puzzleSeed );

                sprintf( puzzleNumberString, "%d", puzzleSeed );
                sprintf( puzzleScoreString, "%d", puzzleScore );
                
                
                printf( "Generating EPS files %s and %s\n",
                        puzzleFileName, solutionFileName );

                File templateFile( NULL, (char *)templateFileName );


                char *puzzleEPSString = templateFile.readFileContents();
                char *solutionEPSString = templateFile.readFileContents();

                if( puzzleEPSString != NULL && solutionEPSString != NULL ) {

                    // map integers in range 1..9 to characters in same range
                    const char *charMap = "0123456789"; 
                    
                    // template spaces are listed in column-major order
                    for( int x=0; x<9; x++ ) {
                        for( int y=0; y<9; y++ ) {
                            int index = y * 9 + x;

                            
                            int puzzleValue = puzzle[ index ];

                            char puzzleChar = ' ';
                            if( puzzleValue > 0 ) {
                                puzzleChar = charMap[ puzzleValue ]; 
                                }
                            
                            // replace next occurrence of (-) in template
                            char *pointer = strstr( puzzleEPSString, "(-)" );

                            if( pointer == NULL ) {
                                printf(
                                    "Didn't find expected (-) in template." );
                                }

                            pointer[1] = puzzleChar;


                            // repeat for solution template
                            
                            int solutionValue = solution[ index ];

                            char solutionChar = ' ';
                            if( solutionValue > 0 ) {
                                solutionChar = charMap[ solutionValue ]; 
                                }
                            
                            // replace next occurrence of (-) in template
                            pointer = strstr( solutionEPSString, "(-)" );

                            if( pointer == NULL ) {
                                printf(
                                    "Didn't find expected (-) in template." );
                                }

                            pointer[1] = solutionChar;

                            
                            }
                        }


                    File puzzleFile( NULL, puzzleFileName );
                    File solutionFile( NULL, solutionFileName );

                    puzzleFile.writeToFile( puzzleEPSString );
                    solutionFile.writeToFile( solutionEPSString );

                    delete [] puzzleEPSString;
                    delete [] solutionEPSString;

                    

                    // now generate latex layout file

                    printf( "Generating latex file %s\n",
                            puzzleLayoutFileName );
                    
                    File latexTemplate( NULL, "layout.tex" );

                    char *latexString = latexTemplate.readFileContents();

                    if( latexString == NULL ) {
                        printf( "Failed to read file layout.tex\n" );
                        }

                    SimpleVector<char *> *targets = new SimpleVector<char *>();
                    SimpleVector<char *> *replacements =
                        new SimpleVector<char *>();

                    targets->push_back( "NUMBER" );
                    replacements->push_back( puzzleNumberString );

                    targets->push_back( "SCORE" );
                    replacements->push_back( puzzleScoreString );

                    targets->push_back( "DIFFICULTY" );

                    // easy: 34-37 hints
                    // medium: 31-33 hints
                    // hard:   27-30 hints

                    // 0   -  60  Easy
                    // 61  - 100  Medium
                    // 101 - 150  Hard
                    // 151 - 300  Intense
                    // 301 +      Severe
                    
                    if( puzzleScore <= 60 ) {
                        replacements->push_back( "Easy" );
                        }
                    else if( puzzleScore <= 100 ) {
                        replacements->push_back( "Medium" );
                        }
                    else if( puzzleScore <= 150 ) {
                        replacements->push_back( "Hard" );
                        }
                    else if( puzzleScore <= 300 ) {
                        replacements->push_back( "Intense" );
                        }
                    else {
                        replacements->push_back( "Severe" );
                        }
                    
                    char *filledLatex =
                        replaceTargetListWithSubstituteList(
                            latexString,
                            targets, replacements );

                    delete targets;
                    delete replacements;

                    File puzzleLayoutFile( NULL, puzzleLayoutFileName );
                    
                    puzzleLayoutFile.writeToFile( filledLatex );

                    delete [] latexString;
                    delete [] filledLatex;



                    
                    char scoreFileName[99];

                    sprintf( scoreFileName, "%d_score.txt", puzzleSeed );

                    printf( "Saving score to file %s\n", scoreFileName );
                    
                    File scoreFile( NULL, scoreFileName );

                    scoreFile.writeToFile( puzzleScoreString );
                    }
                else {
                    printf( "Failed to read file %s\n", templateFileName );
                    }
                }
            }

        puzzleSeed++;

        if( puzzleSeed % 10 == 0 ) {
            printf( "Passing seed %d\n", puzzleSeed );
            }
        }
        
    delete randSource;


    
    
    }



void printPuzzle( int *inPuzzle ) {
    for( int y=0; y<9; y++ ) {

        int lineLength = 0;
        
        int x;
        for( x=0; x<9; x++ ) {
            if( x % 3 == 0 ) {
                printf( "|" );
                }
            else {
                printf( " " );
                }
            lineLength++;
            
            int value = inPuzzle[ y * 9 + x ];

            if( value > 0 ) {
                printf( "%d", value  );
                }
            else if( value == -1 ) {
                printf( "-" );
                }
            else {
                printf( "*" );
                }

            lineLength += 1;

            if( x == 8 ) {
                printf( "|" );
                }
            else {
                printf( " " );
                }
            lineLength++;
            }
        
        printf( "\n" );

        
        if( y % 3 == 2 && y<8) {
            for( x=0; x<lineLength; x++ ) {
            
                printf( "-" );
                }
            printf( "\n" );
            }
        
        }
    }



void printCompactPuzzle( int *inPuzzle ) {
    for( int y=0; y<9; y++ ) {
        for( int x=0; x<9; x++ ) {
            int value = inPuzzle[ y * 9 + x ];

            if( value > 0 ) {
                printf( "%d", value  );
                }
            else if( value == -1 ) {
                printf( "_" );
                }
            else {
                printf( "*" );
                }
            }

        // separate lines with "+"
        if( y<8 ) {
            printf( "+" );
            }
        }
    }



void printValidMap( char *inMap ) {
    for( int y=0; y<9; y++ ) {

        int lineLength = 0;
        
        int x;
        for( x=0; x<9; x++ ) {

            if( x % 3 == 0 ) {
                printf( "|" );
                }
            else {
                printf( " " );
                }
            lineLength++;
            
            
            char value = inMap[ y * 9 + x ];

            if( value == true ) {
                printf( "T" );
                }
            else if( value == 2 ){
                printf( "*" );
                }
            else {
                printf( "-" );
                }
            lineLength++;


            if( x == 8 ) {
                printf( "|" );
                }
            else {
                printf( " " );
                }
            lineLength++;
            }
        printf( "\n" );
        
        if( y % 3 == 2 && y<8) {
            for( x=0; x<lineLength; x++ ) {
            
                printf( "-" );
                }
            printf( "\n" );
            }
        }
    }



char checkPuzzle( int *inPuzzle ) {

    // check that each cell does not conflict with mates
    int mateIndices[20];
    
    for( int i=0; i<81; i++ ) {
        getRowColumnAndBlockmates( i, mateIndices );

        for( int j=0; j<20; j++ ) {
            //printf( "checking mate index %d\n", mateIndices[j] );
            if( inPuzzle[i] == inPuzzle[ mateIndices[j] ] ) {
                // conflict

                int iX = i % 9;
                int iY = i / 9;

                int jX = mateIndices[j] % 9;
                int jY = mateIndices[j] / 9;

                printf( "(%d, %d) conflics with (%d, %d)\n",
                        iX, iY, jX, jY );
                printPuzzle( inPuzzle );
                return false;
                }
            }
        }

    return true;
    }






//  NOTE :

//  Most of the functions below this point are no longer used.
//  the get____Mates functions are one exception (they are still used)







char solvePuzzleForIndex( int *inPuzzle, int inIndex, int inSolveLevel ) {
    if( inPuzzle[ inIndex ] > 0 ) {
        return true;
        }

    char progress = true;

    while( progress ) {
        progress = false;
        
        // first, check if it has only one possible value
        SimpleVector<long> *possibleValues =
            getPossibleValues( inIndex, inPuzzle );

        int numValues = possibleValues->size(); 

        int firstValue = *( possibleValues->getElement( 0 ) );


        if( numValues == 1 ) {
            inPuzzle[inIndex] = firstValue;
            delete possibleValues;
            return true;
            }
        else {

            // mark this cell with -2 so we don't get in a recursion loop
            inPuzzle[inIndex] = -2;
            
            // examine each blank mate and recurse on it
            int mates[20];
            getRowColumnAndBlockmates( inIndex, mates );

            for( int m=0; m<20 && !progress; m++ ) {
                if( inPuzzle[ mates[m] ] == -1 ) {
                    // blank (and unmarked) mate

                    // check if it shares one of our possible values
                    char sharedValue = false;

                    SimpleVector<long> *matePossibleValues =
                        getPossibleValues( mates[m], inPuzzle );

                    int numMateValues = matePossibleValues->size();

                    for( int a=0; a<numValues && !sharedValue; a++ ) {
                        for( int b=0; b<numMateValues && !sharedValue; b++ ) {
                            int value = *( possibleValues->getElement(a) );
                            int mateValue =
                                *( matePossibleValues->getElement(b) );

                            if( mateValue == value ) {
                                sharedValue = true;
                                }
                            }
                        }
                    delete matePossibleValues;

                    if( sharedValue ) {
                        progress = solvePuzzleForIndex( inPuzzle, mates[m],
                                                        inSolveLevel );
                        }
                    // else skip mate, since it doesn't affect us
                    }
                }

            // back to blank
            inPuzzle[inIndex] = -1;
            }
        
        delete possibleValues;
        }
    
    // no progress
    return false;
    }



int countPossibleValues( char inValueFlags[9] ) {
    int count = 0;
    for( int v=0; v<9; v++ ) {
        if( inValueFlags[v] ) {
            count++;
            }
        }
    return count;
    }



int getFirstPossibleValue( char inValueFlags[9] ) {
    for( int v=0; v<9; v++ ) {
        if( inValueFlags[v] ) {
            return v+1;
            }
        }
    return -1;
    }



/**
 * Clears a flag array and sets only one true flag
 *
 * @param inValueFlags a true/false array.
 *  Destroyed by caller.
 * @param inValue the only possible value to set.
 */
void setOnlyPossibleValue( char inValueFlags[9], int inValue ) {
    // first set all to false
    for( int v=0; v<9; v++ ) {
        inValueFlags[v] = false;
        }
    // now set lone value to true
    inValueFlags[ inValue - 1 ] = true;   
    }


/**
 * Updates a possible value map when a particular cell is fixed at a single
 * value.
 *
 * @param inMap the possible value map.
 *  Destroyed by caller.
 * @param inIndex the index of the cell.
 * @param inSingleValue the value of the cell.
 */
void updatePossibleValueMap( char inMap[81][9],
                             int inIndex, int inSingleValue ) {
    // remove this value from all mates

    int mates[20];
                    
    getRowColumnAndBlockmates( inIndex, mates );

    int valueIndex = inSingleValue - 1;
    for( int m=0; m<20; m++ ) {
        inMap[ mates[m] ][ valueIndex ] = false;
        }
    }



char solvePuzzle( int *inPuzzle, int inSolveLevel, int inStopWhenIndexSet ) {

    // track possible values for each cell across all work below
    // thus, we don't repeat work

    // possibleValues[i][v-1] is true if v is a possible value for cell i 
    char possibleValues[81][9];

    // fill in loose possible value list for each cell
    for( int i=0; i<81; i++ ) {
        // set all to false
        int v;
        for( v=0; v<9; v++ ) {
            possibleValues[i][v] = false;
            }

        // set to true for cell's possible values
        SimpleVector<long> *valueVector =
            getPossibleValues( i, inPuzzle );

        for( int v=0; v<valueVector->size(); v++ ) {
            int value = *( valueVector->getElement( v ) );

            possibleValues[i][ value - 1 ] = true;
            }
                
        delete valueVector;
        }


    
    char progress = true;

    int blankCount = 0;
    while( progress ) {

        if( inStopWhenIndexSet != -1 &&
            inPuzzle[ inStopWhenIndexSet ] > 0 ) {
            // found a value for the cell we are interested in.
            
            return true;
            }

        // construct valid maps for each value given current puzzle state
        // reuse these maps below in loop over all cells
        // these maps will become stale as values are filled in, but the maps
        // will be too conservative, so there is no danger of entering an
        // invalid state

        // why do this?  Profiler discovered lots of calls to
        // findValidPlacesFor
        // Before this optimization:  5900 calls (1.47 sec total run time)
        // After:  963 calls (0.29 sec runtime)
        char **validMaps = new char*[9];

        int v;
        for( v=0; v<9; v++ ) {
            validMaps[v] = new char[81];

            findValidPlacesFor( v+1, inPuzzle, validMaps[v] );
            }
        
        blankCount = 0;
        progress = false;
        // check each square
        for( int i=0; i<81; i++ ) {
            if( inPuzzle[i] == -1 ) {
                // a blank cell
                blankCount++;

                // first, check if it has only one possible value

                int numValues = countPossibleValues( possibleValues[i] ); 
                char foundValue = false;
                if( numValues == 1 ) {
                    int value = getFirstPossibleValue( possibleValues[i] );
                    inPuzzle[i] = value;

                    updatePossibleValueMap( possibleValues, i, value );
                    progress = true;
                    foundValue = true; 
                    }

                // if not, consider solver level 1 (using valid maps)
                if( !foundValue && inSolveLevel > 0 ) {
                    // solver level 1:
                    // Check if this cell is the only valid location
                    // for one of its possible values in its row, column,
                    // or group

                    for( v=0; v<9 && !foundValue; v++ ) {

                        if( possibleValues[i][v] ) {
                            
                            // valid map may be a bit stale, but use it anyway
                            foundValue = isOnlyValid( i, validMaps[ v ] );

                            if( foundValue ) {
                                int value = v+1;
                                inPuzzle[i] = value;
                                setOnlyPossibleValue( possibleValues[i],
                                                      value );
                                updatePossibleValueMap( possibleValues, i,
                                                        value ); 
                                progress = true;
                                }
                            }
                        }
                    }

                // solver level 2
                // N cells that collectively must contain N values
                if( !foundValue && inSolveLevel > 1 ) {
                    //printf( "Level 2\n" );
                    int mateSets[3][8];
                    int s;
                    
                    getRowmates( i, mateSets[0] );
                    getColumnmates( i, mateSets[1] );
                    getBlockmates( i, mateSets[2] );

                    int v;
                    
                    // for each mate set
                    for( s=0; s<3; s++ ) {
                        // 8-bit vector represents a subset of a mate set
                        unsigned char bitVector;
                        // loop over all possible bit vectors (except 0)
                        // thus looping over all possible non-empty subsets
                        // loop from 1 to 255
                        // stop when we wrap back around to 0
                        for( bitVector = 1; bitVector != 0; bitVector++ ) {
                            //printf( "Bit vector = %hhd\n", bitVector );

                            // track allowed values using a flag array
                            char netAllowedValues[8] =
                                { false, false, false, false,
                                  false, false, false, false };
                            
                            int subsetSize = 0;
                            for( int m=0; m<8; m++ ) {
                                char mateInSubset = (bitVector >> m) & 0x01;

                                if( mateInSubset ) {
                                    subsetSize++;

                                    char *matePossibleValues =
                                        possibleValues[ mateSets[s][m] ];
                                    
                                    // add values to full value set
                                    for( v=0; v!=9; v++ ) {
                                        if( matePossibleValues[v] ) {
                                            netAllowedValues[v] = true;
                                            }
                                        }
                                    }
                                }

                            int numValues = 0;
                            for( v=0; v<9; v++ ) {
                                if( netAllowedValues[v] ) {
                                    numValues++;
                                    }
                                }
                            
                            if( subsetSize == numValues ) {
                                // subset same size as total number of
                                // allowed values in the set
                                // Thus, these values *must* occur somewhere
                                // in this subset

                                
                                for( v=0; v<9; v++ ) {
                                    if( netAllowedValues[v] ) {
                                        // Thus, they cannot occur in our cell
                                        possibleValues[i][v] = false;

                                        // also, the cannot occur in the
                                        // non-subset mates
                                        for( int m=0; m<8; m++ ) {
                                            char mateInSubset =
                                                (bitVector >> m) & 0x01;

                                            if( !mateInSubset ) {
                                                // a mate that is not in the
                                                // subset
                                                possibleValues
                                                    [mateSets[s][m]]
                                                    [v] = false;
                                                }
                                            }
                                        }
                                    }

                                
                                }
                            }
                        }
                    
                    // check if we have now reduced our possible values to 1

                    numValues = countPossibleValues( possibleValues[i] ); 
                    if( numValues == 1 ) {
                        int value =
                            getFirstPossibleValue( possibleValues[i] ); 

                        inPuzzle[i] = value;
                        
                        updatePossibleValueMap( possibleValues, i, value );
                        
                        progress = true;
                        foundValue = true;
                        }
                    }
                }
            }

        // clean up valid maps
        for( v=0; v<9; v++ ) {
            delete [] validMaps[v];
            }
        delete [] validMaps;
        
        }
    
    if( blankCount != 0 ) {
        return false;
        }
    else {
        return true;
        }
    }



char findValidPlacesFor( int inValue, int *inPartialPuzzle, 
                         char *outValidMap ) {
    char returnValue = false;

    int mateIndices[20];
    int i;
    
    // check each square
    for( i=0; i<81; i++ ) {
            
        // this square not yet filled in
        if( inPartialPuzzle[ i ] == -1 ) {

            char foundConflict = false;
            
            getRowColumnAndBlockmates( i, mateIndices );
            
            for( int j=0; j<20 && !foundConflict; j++ ) {

                if( inValue == inPartialPuzzle[ mateIndices[j] ] ) {
                    outValidMap[ i ] = false;
                    foundConflict = true;
                    }
                }
            
            if( !foundConflict ) {
                outValidMap[ i ] = true;
                returnValue = true;
                }
            }
        else {
            // already filled in
            outValidMap[ i ] = false;
            }
        }

    // now we have a map of possible valid places, but this map
    // may contain conflicts
    // For example:

    /*
      Failed to remove value 1 from position (1, 4)
      |6  4  7 |9  -  8 |5  -  1|
      |2  8  - |7  5  - |9  4  -|
      |3  -  9 |4  1  - |2  8  7|
      ---------------------------
      |-  -  - |6  -  2 |8  -  -|
      |-  1  6 |3  8  - |-  2  9|
      |9  2  8 |5  4  1 |6  -  3|
      ---------------------------
      |7  -  4 |2  6  9 |1  5  8|
      |-  9  - |8  3  4 |-  6  -|
      |8  6  2 |1  7  5 |3  -  4|
      
      Other valid places for 1:
      |-  -  - |-  -  - |-  -  -|
      |-  -  T |-  -  - |-  -  -|
      |-  -  - |-  -  - |-  -  -|
      ---------------------------
      |T  T  T |-  -  - |-  T  -|
      |T  *  - |-  -  - |-  -  -|
      |-  -  - |-  -  - |-  -  -|
      ---------------------------
      |-  -  - |-  -  - |-  -  -|
      |T  -  T |-  -  - |-  -  -|
      |-  -  - |-  -  - |-  -  -|
    */

    // Notice that the T in the upper-left block is the only T in that block
    // That means that it *must* be a 1 in the puzzle solution.
    // So, it wipes out the other T's in that column.
    // Same goes for the T in the middle-right block---it wipes out the other
    // T's in that row.

    // clean up the map with this function

    // let caller invoke this externally
    // removeConflictsFromValidMap( outValidMap );
    
    return returnValue;
    }



char isOnlyValid( int inIndex, char *inValidMap ) {
    // reuse this array to represent our mate group in each test below
    int mateList[8];
    int i;
    
    char foundInRow = false;
    char foundInColumn = false;
    char foundInBlock = false;
                
    getRowmates( inIndex, mateList );
    
    for( i=0; i<8 && !foundInRow; i++ ) {
        foundInRow = inValidMap[ mateList[i] ];
        }

    getColumnmates( inIndex, mateList );
                                
    for( i=0; i<8 && !foundInColumn; i++ ) {
        foundInColumn = inValidMap[ mateList[i] ];
        }

    getBlockmates( inIndex, mateList );
                                
    for( i=0; i<8 && !foundInBlock; i++ ) {
        foundInBlock = inValidMap[ mateList[i] ];
        }

    if( !( foundInRow && foundInColumn && foundInBlock ) ) {
        // we are the only valid cell in at least one of the following:
        // our row, our column, or our block
        return true;
        }
    else {
        return false;
        }
    }



void removeConflictsFromValidMap( char *inValidMap, char inUseAdvanced ) {

    int i;

    // reused in loop
    int mateIndices[20];

    // repeat until there is no change

    char wasChange = true;
    int repeatCount = 0;

    while( wasChange ) {
        // check for change again this time
        wasChange = false;
        
        // check each square
        for( i=0; i<81; i++ ) {
            if( inValidMap[i] &&
                isOnlyValid( i, inValidMap ) ) {
                
                // this position *must* contain the value in question

                // thus, the value in question is not actually valid
                // in the rest of our row, the rest of our column, or the rest
                // of our block

                // thus, mark all mates as not valid spots for the value
                getRowColumnAndBlockmates( i, mateIndices );

                for( int j=0; j<20; j++ ) {
                    if( inValidMap[ mateIndices[j] ] ) {
                        // marked as valid
                        // change to invalid
                        inValidMap[ mateIndices[j] ] = false;
                        // note the change
                        wasChange = true;
                        }
                    }
                }
            else if( inUseAdvanced && inValidMap[i] ) {
                // this cell is valid, but it's not the only valid cell
                // in its row, column, or block

                int rowmates[8];
                int columnmates[8];
                int blockmates[8];
                getRowmates( i, rowmates );
                getColumnmates( i, columnmates );
                getBlockmates( i, blockmates );

                // if all valid cells in our row are also in our block,
                // the other valid cells in our block cannot be valid

                char change = false;
                
                change = change ||
                    removeOverlappingMateConflicts( inValidMap, rowmates,
                                                    blockmates );
                // same for column vs. block
                change = change ||
                    removeOverlappingMateConflicts( inValidMap, columnmates,
                                                    blockmates );
                // now block vs. row
                change = change ||
                    removeOverlappingMateConflicts( inValidMap, blockmates,
                                                    rowmates );
                // now block vs. column
                change = change ||
                    removeOverlappingMateConflicts( inValidMap, blockmates,
                                                    columnmates );

                wasChange = change;
                }
            }
        repeatCount ++;
        }
    }



char removeOverlappingMateConflicts( char *inValidMap, int *inMateSetA,
                                     int *inMateSetB ) {

    char allSetAValidsInSetB = true;

    // set A and B overlap in at most 2 places
    int overlappingMateCount = 0;
    int overlappingMates[2];

    for( int a=0; a<8 && allSetAValidsInSetB; a++ ) {
        if( inValidMap[ inMateSetA[a] ] ) {
            allSetAValidsInSetB = false;
            for( int b=0; b<8; b++ ) {
                if( inMateSetA[a] == inMateSetB[b] ) {
                    // this rowmate is in our block
                    overlappingMates[ overlappingMateCount ] =
                        inMateSetA[a];
                    overlappingMateCount++;
                                
                    allSetAValidsInSetB = true;
                    }
                }
            }
        }

    char change = false;
    
    if( allSetAValidsInSetB ) {
        // remove other valids from set B
        // (the non-overlapping ones)
        for( int b=0; b<8; b++ ) {
            int setBMateIndex = inMateSetB[b]; 
            if( setBMateIndex != overlappingMates[0] &&
                setBMateIndex != overlappingMates[1] ) {

                if( inValidMap[ setBMateIndex ] ) {
                    inValidMap[ setBMateIndex ] = false;
                    change = true;
                    }
                }
            }
        }

    return change;
    }



int countPossibleValues( int inIndex, int *inPartialPuzzle ) {
    SimpleVector<long> *values = getPossibleValues( inIndex, inPartialPuzzle );

    int count = values->size();
    delete values;
    
    return count;
    }



SimpleVector<long> *getPossibleValues( int inIndex, int *inPartialPuzzle ) {

    if( inPartialPuzzle[ inIndex ] > 0 ) {
        // value already set for this cell
        // thus:  one possible value
        SimpleVector<long> *returnVector = new SimpleVector<long>();
        returnVector->push_back( inPartialPuzzle[ inIndex ] );

        return returnVector;
        }
    
    int mateIndices[20];

    getRowColumnAndBlockmates( inIndex, mateIndices );

    // track which values are still allowed
    char allowedValueFlags[9];

    int i;
    for( i=0; i<9; i++ ) {
        allowedValueFlags[i] = true;
        }

    // look at each mate's values and clear allowedValueFlags accordingly
    for( i=0; i<20; i++ ) {
        int value = inPartialPuzzle[ mateIndices[i] ];

        if( value > 0 ) {
            allowedValueFlags[ value - 1 ] = false;
            }
        }

    // assemble values into a vector
    SimpleVector<long> *returnVector = new SimpleVector<long>();
    
    for( i=0; i<9; i++ ) {
        if( allowedValueFlags[i] ) {
            returnVector->push_back( i + 1 );
            }
        }
    
    return returnVector;
    }



char checkForConflict( int *inPartialPuzzle, int *inRecursionLimit ) {
    if( *inRecursionLimit == 0 ) {
        // run out of steps
        // no conflict found
        return false;
        }
    // this call counts as one step
    (*inRecursionLimit) --;
    
    //printPuzzle( inPartialPuzzle );
    // check each cell and return if a conflict found

    // make a copy of inPartialPuzzle so that we can memoize values
    // when we discover blank cells that have only one possible value
    int *puzzleCopy = copyPuzzle( inPartialPuzzle );

    int i;
    
    // first, go through and set any blanks with lone values
    // also check for blanks that have no possible values
    // repeat until no change
    char change = true;
    while( change ) {
        change = false;
        for( i=0; i<81; i++ ) {
            if( puzzleCopy[i] == -1 ) {
                // a blank cell
            
                // what values are possible for it?
                SimpleVector<long> *values =
                    getPossibleValues( i, puzzleCopy );

                int numValues = values->size();

                if( numValues == 0 ) {
                    // conflict right away
                    delete values;
                    delete [] puzzleCopy;
                    return true;
                    }
                else if( numValues == 1 ) {
                    puzzleCopy[i] = *( values->getElement( 0 ) );
                    change = true;
                    }
                delete values;
                }
            }
        }
    
    // now check for set values that are already in conflict
    for( i=0; i<81; i++ ) {
        if( puzzleCopy[i] > 0 ) {

            // make sure the value is allowed
            SimpleVector<long> *values =
                getPossibleValues( i, puzzleCopy );

            int numValues = values->size();
            
            char found = false;
            for( int j=0; j<numValues && !found; j++ ) {
                long value = *( values->getElement( j ) );

                if( value == puzzleCopy[i] ) {
                    found = true;
                    }
                }

            delete values;

            if( ! found ) {
                // the set value is not allowed
                delete [] puzzleCopy;
                return true;
                }
            }
        }

    
    // no conflict found so far
    
    // now recurse on blank cells to test their possible values
    for( i=0; i<81; i++ ) {

        if( puzzleCopy[i] == -1 ) {
            // a blank cell

            // what values are possible for it?
            SimpleVector<long> *values =
                getPossibleValues( i, puzzleCopy );

            int numValues = values->size();

            // need to make sure all possible values result in a conflict
            // before we declare a conflict at this cell
            char foundConflict = true;


            // try each value in turn and recursively check for conflict
            // if there are 0 values, foundConflict will remain true
            for( int j=0; j<numValues && foundConflict; j++ ) {
                long value = *( values->getElement( j ) );
                
                puzzleCopy[i] = value;
                
                foundConflict = checkForConflict( puzzleCopy,
                                                  inRecursionLimit );
                
                // back to blank --- puzzle now restored to original state
                puzzleCopy[i] = -1;                    
                }
                
            delete values;            
            
            if( foundConflict ) {
                // all possible values lead to a conflict
                // or no values were possible
                delete [] puzzleCopy;
                return true;
                }
            }
        }

    // no conflict found
    return false;
    }



void getRowmates( int inIndex, int *outRowMateIndices ) {
    int x = inIndex % 9 ;
    int y = inIndex / 9;

    int mateListIndex = 0;

    // add surrounding row
    for( int x1=0; x1<9; x1++ ) {
        if( x != x1 ) {
            outRowMateIndices[ mateListIndex ] =  y * 9 + x1;
            mateListIndex ++;
            }
        }
    }



void getColumnmates( int inIndex, int *outRowMateIndices ) {
    int x = inIndex % 9 ;
    int y = inIndex / 9;

    int mateListIndex = 0;
    
    // add surrounding column
    for( int y1=0; y1<9; y1++ ) {
        if( y != y1 ) {
            outRowMateIndices[ mateListIndex ] = y1 * 9 + x;
            mateListIndex ++;
            }
        }
    }



void getBlockmates( int inIndex, int *outBlockMateIndices ) {
    int x = inIndex % 9 ;
    int y = inIndex / 9;

    // add surrounding block

    // these four deltas away from (x, y) determine all 8 other
    // cells in our block
    int yDeltaA = 0;
    int yDeltaB = 0;
    int xDeltaA = 0;
    int xDeltaB = 0;

    // ignore 4 of them (those already in our column and row)
    
    
    // these deltas are set depending on where we are in our block

    int indexMod3 = inIndex % 3;
    switch( indexMod3 ) {
        case 0:
            // first column of block
            xDeltaA = 1;
            xDeltaB = 2;
            break;
        case 1:  
            // second column of block
            xDeltaA = -1;
            xDeltaB = 1;
            break;
        case 2:
            // third column of block
            xDeltaA = -1;
            xDeltaB = -2;
            break;
        default:
            printf( "error in switch( indexMod3 )\n" );
            break;
        }

    int yTest = ( inIndex % 27 ) / 9;

    switch( yTest ) {
        case 0:
            // first row of block
            yDeltaA = 1;
            yDeltaB = 2;
            break;
        case 1:  
            // second row of block
            yDeltaA = -1;
            yDeltaB = 1;
            break;
        case 2:
            // third row of block
            yDeltaA = -1;
            yDeltaB = -2;
            break;
        default:
            printf( "error in switch( yTest )\n" );
            break;
        }

    // positions of our cell's block-mates (ignoring those in our row/column)
    int xPositionsInBlock[8];
    int yPositionsInBlock[8]; 

    

    xPositionsInBlock[0] = x + xDeltaA;
    yPositionsInBlock[0] = y + yDeltaA;

    xPositionsInBlock[1] = x + xDeltaA;
    yPositionsInBlock[1] = y + yDeltaB;

    xPositionsInBlock[2] = x + xDeltaB;
    yPositionsInBlock[2] = y + yDeltaA;

    xPositionsInBlock[3] = x + xDeltaB;
    yPositionsInBlock[3] = y + yDeltaB;

    xPositionsInBlock[4] = x;
    yPositionsInBlock[4] = y + yDeltaA;

    xPositionsInBlock[5] = x;
    yPositionsInBlock[5] = y + yDeltaB;

    xPositionsInBlock[6] = x + xDeltaA;
    yPositionsInBlock[6] = y;

    xPositionsInBlock[7] = x + xDeltaB;
    yPositionsInBlock[7] = y;

    
    // add blockmates 
    for( int b=0; b<8; b++ ) {
        int x1 = xPositionsInBlock[ b ];
        int y1 = yPositionsInBlock[ b ];
                    
        outBlockMateIndices[b] = y1 * 9 + x1;
        }
    }



void getRowColumnAndBlockmates( int inIndex, int *outBlockMateIndices ) {
    
    // add surrounding column
    getColumnmates( inIndex, outBlockMateIndices );

    
    // add surrounding row
    getRowmates( inIndex, &( outBlockMateIndices[8] ) );
    
    // add surrounding block (only use first four elements to avoid overlap
    // with row and colum
    int blockMates[8];
    getBlockmates( inIndex, blockMates );

    for( int i=0; i<4; i++ ) {
        outBlockMateIndices[16 + i] = blockMates[i];
        }
    }



void randomRename( int *inPuzzle, RandomSource *inRandSource ) {
    // got this idea from here:
    // http://www.warebizprogramming.com/text/cpp/section6/part8.htm
    
    int i;

    // first, map all symbols into the teens (1 -> 11, 5 -> 15, etc.)
    for( i=0; i<81; i++ ) {
        inPuzzle[i] += 10;
        }

    // construct a symbol mapping table
    // m(x) = table[ x - 1 ] 

    int table[9];

    // build the identity mapping
    for( i=0; i<9; i++ ) {
        table[i] = i + 1;
        }

    // now shuffle it

    //      move a random element from index range [0..7] to position 8
    // then move a random element from index range [0..6] to position 7

    for( i=8; i>=0; i-- ) {

        int swapIndex = inRandSource->getRandomBoundedInt( 0, i-1 );

        int temp = table[i];
        table[i] = table[swapIndex];
        table[swapIndex] = temp;
        }

    // apply mapping
    for( i=0; i<81; i++ ) {
        int oldValue = inPuzzle[i] - 10;

        // map it
        inPuzzle[i] = table[ oldValue - 1 ];
        }
    
    }



void swapRows( int *inPuzzle, int inRowA, int inRowB ) {

    for( int x=0; x<9; x++ ) {
        int temp = inPuzzle[ inRowA * 9 + x ];
        inPuzzle[ inRowA * 9 + x ] = inPuzzle[ inRowB * 9 + x ];

        inPuzzle[ inRowB * 9 + x ] = temp;        
        }
    }



void swapColumns( int *inPuzzle, int inColumnA, int inColumnB ) {

    for( int y=0; y<9; y++ ) {
        int temp = inPuzzle[ y * 9 + inColumnA ];
        inPuzzle[ y * 9 + inColumnA ] = inPuzzle[ y * 9 + inColumnB ];

        inPuzzle[ y * 9 + inColumnB ] = temp;        
        }
    }



void swapRowBlocks( int *inPuzzle, int inRowBlockA, int inRowBlockB ) {

    // swap each row in block
    for( int i=0; i<3; i++ ) {
        swapRows( inPuzzle, inRowBlockA * 3 + i, inRowBlockB * 3 + i );
        }
    }



void swapColumnBlocks( int *inPuzzle, int inColumnBlockA,
                       int inColumnBlockB ) {
    
    // swap each column in block
    for( int i=0; i<3; i++ ) {
        swapColumns( inPuzzle, inColumnBlockA * 3 + i,
                     inColumnBlockB * 3 + i );
        }
    }


