
/**
 * Copies a puzzle.
 *
 * @param inPuzzle the puzzle to copy.
 *   Destroyed by caller.
 *
 * @return a newly allocated copy of the inPuzzle.
 *   Destroyed by caller.
 */
int *copyPuzzle( int *inPuzzle );

    

/**
 * Prints a puzzle to standard out.
 *
 * @param inPuzzle the puzzle to print.  Destroyed by caller.
 */
void printPuzzle( int *inPuzzle );



/**
 * Prints a compact, one-line representation of puzzle as used here:
 * http://www.sudokusolver.co.uk/
 *
 * @param inPuzzle the puzzle to print.  Destroyed by caller.
 */
void printCompactPuzzle( int *inPuzzle );



/**
 * Gets the indices of all cells that are in the same row, column, or block
 * as a given cell.  List contains exactly 20 indices with no repeats.
 *
 * @param inIndex the index in range [0..80] to get mates for.
 * @param outBlockMateIndices a 20-index array that will be filled in
 *   by this call.  Must be allocated and destroyed by caller.
 */
void getRowColumnAndBlockmates( int inIndex, int *outBlockMateIndices );



/**
 * Same as getRowColumnAndBlockmates, but with an 8-element array.
 */
void getRowmates( int inIndex, int *outRowMateIndices );
void getColumnmates( int inIndex, int *outRowMateIndices );



/**
 * Same as getRowmates, with an 8-element array.
 *
 * First 4 elements in array are those blockmates that aren't in this
 * element's row or column.
 */
void getBlockmates( int inIndex, int *outRowMateIndices );



/**
 * Counts the number of true elements in a flag array.
 *
 * @param inValueFlags a true/false array.
 *  Destroyed by caller.
 *
 * @return the number of values flagged as true.
 */
int countPossibleValues( char inValueFlags[9] );



/**
 * Gets the first value flagged in an array.
 *
 * @param inValueFlags a true/false array.
 *  Destroyed by caller.
 *
 * @return the first value flagged as true, or -1 if none flagged
 */
int getFirstPossibleValue( char inValueFlags[9] );



/**
 * Clears a flag array and sets only one true flag
 *
 * @param inValueFlags a true/false array.
 *  Destroyed by caller.
 * @param inValue the only possible value to set.
 */
void setOnlyPossibleValue( char inValueFlags[9], int inValue );
