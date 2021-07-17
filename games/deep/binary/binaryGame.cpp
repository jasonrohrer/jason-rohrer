#include "minorGems/util/SimpleVector.h"
#include "minorGems/util/random/JenkinsRandomSource.h"
#include "minorGems/system/Time.h"
#include "minorGems/system/Thread.h"


void printTable();
void colAPicked();
void colBPicked();
void timedComputerMove();
void pickComputerMove();
int worstCaseScore( SimpleVector<int> *inA, int inAIndex,
                    SimpleVector<int> *inB, int inBIndex,
                    int inTurn );

void findInterestingStates( int inColumnLength );



int numRounds = 14;

SimpleVector<int> colA;
SimpleVector<int> colB;

SimpleVector<int> colAOrig;
SimpleVector<int> colBOrig;

int humanScore = 0;
int machineScore = 0;

JenkinsRandomSource randSource( 1343 );


int vectorPopFront( SimpleVector<int> *inV ) {
    int val = inV->getElementDirect( 0 );
    inV->deleteElement( 0 );
    return val;
    }



static void printPrompt() {
    printf( "\nPick A or B: " );
    }


int main() {
    Time::getCurrentTime();

    findInterestingStates( 7 );
    
    for( int i=0; i<numRounds; i++ ) {
        if( randSource.getRandomBoolean() ) {
            colA.push_back( 1 );
            }
        else {
            colA.push_back( 0 );
            }
        if( randSource.getRandomBoolean() ) {
            colB.push_back( 1 );
            }
        else {
            colB.push_back( 0 );
            }
        }

    colAOrig.push_back_other( &colA );
    colBOrig.push_back_other( &colB );

    printTable();
    printPrompt();
    

    int numRead = 1;
    while( numRead == 1 && ( colA.size() > 0 || colB.size() > 0 ) ) {
        
        char buffer[100];
        numRead = scanf( "%99s", buffer );
        
        if( numRead == 1 ) {
            if( colA.size() > 0 &&
                ( strcmp( buffer, "a" ) == 0 ||
                  strcmp( buffer, "A" ) == 0  ) ) {
                colAPicked();
                }
                
            else if( colB.size() > 0 &&
                     ( strcmp( buffer, "b" ) == 0 ||
                       strcmp( buffer, "B" ) == 0 ) ) {
                colBPicked();
                }
            else {
                printf( "Invalid choice: %s\n", buffer );
                printPrompt();
                }
            }
        }
    
    }


char machinePickBuffer[99];

const char *machinePick = "";



void printTable() {
    if( strcmp( machinePick, "" )  == 0 ) {
        printf( "\n" );
        }
    else {
        printf( "%s\n\n", machinePick );
        }
    printf( "Human: %d       Computer: %d\n", humanScore, machineScore );    
    printf( "A:  " );

    for( int i=0; i<colA.size(); i++ ) {
        
        int v = colA.getElementDirect( i );
        
        printf( "%d  ", v );
        }
    printf( "\nB:  " );

    for( int i=0; i<colB.size(); i++ ) {
        
        int v = colB.getElementDirect( i );
        
        printf( "%d  ", v );
        }
    printf( "\n" );
    }


void colAPicked() { 
	int v = vectorPopFront( &colA );
    
	humanScore += v;
	machinePick = "";
    printf( "\nHuman picks A\n" );
	printTable();
    printf( "\n" );
	timedComputerMove();
    }


void colBPicked() { 	
	int v = vectorPopFront( &colB );

	humanScore += v;
	machinePick = "";
    printf( "\nHuman picks B\n" );
	printTable();
    printf( "\n" );
	timedComputerMove();
    }



void timedComputerMove() {
	double startTime = Time::getCurrentTime();

	pickComputerMove();
	double endTime = Time::getCurrentTime();

	double elapsed = endTime - startTime;

	double targetTime = 2;
	if( endTime - startTime < targetTime ) {
        Thread::staticSleep( 1000 * (targetTime - elapsed ) );
        }
    printTable();
    printPrompt();
    }



SimpleVector<int> removeFrontAndCopy( SimpleVector<int> *inV ) {
    SimpleVector<int> vRemain;
    vRemain.push_back_other( inV );
    
    vectorPopFront( &vRemain );
    return vRemain;
    }

    


void pickComputerMove() {

	int v = 0;
	
	if( colA.size() == 0 && colB.size() == 0 ) {
		return;
        }
	else if( colA.size() == 0 ) {
		v = vectorPopFront( &colB );
		machinePick = "Machine picks B";
        }
	else if( colB.size() == 0 ) {
        v = vectorPopFront( &colA );
		machinePick = "Machine picks A";
        }
	else {

        
		int worstScoreIfPickA = 
			worstCaseScore( &colA, 1, &colB, 0, 0 ) + 
            colA.getElementDirect( 0 );

		int worstScoreIfPickB = 
			worstCaseScore( &colA, 0, &colB, 1, 0 ) + 
            colB.getElementDirect(0);
	
		if( worstScoreIfPickA > worstScoreIfPickB ) {
			v = vectorPopFront( &colA );
			sprintf( machinePickBuffer, "Machine picks A (%d, %d)", 
                     worstScoreIfPickA,
                     worstScoreIfPickB );
            machinePick = machinePickBuffer;
            }
		else {
            v = vectorPopFront( &colB );
			sprintf( machinePickBuffer, "Machine picks B (%d, %d)", 
                     worstScoreIfPickA,
                     worstScoreIfPickB );
			machinePick = machinePickBuffer;
            }
        }
	machineScore += v;
    }



int worstCaseScore( SimpleVector<int> *inA, int inAIndex, 
                    SimpleVector<int> *inB, int inBIndex, 
                    int inTurn ) {
    
    int aSize = inA->size();
    int bSize = inB->size();
    

    if( aSize == inAIndex && bSize == inBIndex ) {
		return 0;
		}
	// if we're down to only one column
	// top element goes to whoever's turn it is.
	else if( aSize == inAIndex ) {
        int b0 = inB->getElementDirect( inBIndex );

		if( inTurn ) {
			return worstCaseScore( inA, inAIndex, 
                                   inB, inBIndex + 1,
                                   0 ) + b0;
			}
		else {
			return worstCaseScore( inA, inAIndex, 
                                   inB, inBIndex + 1,
                                   1 ) - b0;
            }
        }
	else if( bSize == inBIndex ) {
        int a0 = inA->getElementDirect( inAIndex );
        
		if( inTurn ) {
			return worstCaseScore( inA, inAIndex + 1, 
                                   inB, inBIndex,
                                   0 ) + a0;
			}
		else {
			return worstCaseScore( inA, inAIndex + 1, 
                                   inB, inBIndex,
                                   1 ) - a0;
            }
        }
	else {
		// neither empty
        int a0 = inA->getElementDirect( inAIndex );
        int b0 = inB->getElementDirect( inBIndex );
        
		if( inTurn ) {
			// our turn
			int worstIfPickA = worstCaseScore( inA, inAIndex + 1, 
                                               inB, inBIndex,
                                               0 ) + a0;
			int worstIfPickB = worstCaseScore( inA, inAIndex,
                                               inB, inBIndex + 1, 
                                               0 ) + b0;
			
			// we want to maximize our worst-case score after this
			if( worstIfPickA > worstIfPickB ) {
				return worstIfPickA;
                }
			else {
				return worstIfPickB;
                }
            }
		else {
			// their turn
			int worstIfPickA = worstCaseScore( inA, inAIndex + 1, 
                                               inB, inBIndex,
                                               1 ) - a0;
			int worstIfPickB = worstCaseScore( inA, inAIndex,
                                               inB, inBIndex + 1, 
                                               1 ) - b0;
			
			// they want to minimize our worst-case score after this
			if( worstIfPickA < worstIfPickB ) {
				return worstIfPickA;
                }
			else {
				return worstIfPickB;
                }
            }
        }
    }



/*
  function rewind() {
  humanScore = 0;
  machineScore = 0;
  machinePick = "";
  colA = colAOrig.slice();
  colB = colBOrig.slice();
  setupTable();
  }
*/


static void addOne( SimpleVector<int> *inA, SimpleVector<int> *inB ) {
    *( inA->getElement( 0 ) ) += 1;
    
    int i = 0;
    while( i < inA->size() - 1 &&
           inA->getElementDirect( i ) == 2 ) {
        *( inA->getElement( i ) ) -= 2;
        *( inA->getElement( i + 1 ) ) += 1;
        i++;
        }
    
    if( inA->getElementDirect( inA->size() - 1 ) == 2 ) {
        // carry into B
        *( inA->getElement( inA->size() - 1 ) ) -= 2;
    
        *( inB->getElement( 0 ) ) += 1;

        int i = 0;
        while( i < inB->size() - 1 &&
               inB->getElementDirect( i ) == 2 ) {
            *( inB->getElement( i ) ) -= 2;
            *( inB->getElement( i + 1 ) ) += 1;
            i++;
            }
        }
    }


static void print( SimpleVector<int> *inA, SimpleVector<int> *inB ) {
    printf( "A: " );
    for( int i=0; i< inA->size(); i++ ) {
        printf( "%d  ", inA->getElementDirect( i ) );
        }
    printf( "\n" );
    
    printf( "B: " );
    for( int i=0; i< inB->size(); i++ ) {
        printf( "%d  ", inB->getElementDirect( i ) );
        }
    printf( "\n\n" );    
    }



void findInterestingStates( int inColumnLength ) {
    SimpleVector<int> testA;
    SimpleVector<int> testB;

    // both start at 0
    // each iteration, we line them up, end-to-end, and add 1 to first
    // position, like binary addition with carries
    // thus, we walk through all possible states for A and B
    for( int i=0; i<inColumnLength; i++ ) {
        testA.push_back( 0 );
        testB.push_back( 0 );
        }
    

    // we overflow at the end
    while( testB.getElementDirect( testB.size() - 1 ) != 2 ) {
        
        int worstScoreIfPickA = 
            worstCaseScore( &testA, 1, &testB, 0, 0 ) + 
            testA.getElementDirect( 0 );

        int worstScoreIfPickB = 
            worstCaseScore( &testA, 0, &testB, 1, 0 ) + 
            testB.getElementDirect(0);

        if( testA.getElementDirect( 0 ) != testB.getElementDirect( 0 ) ) {
            if( worstScoreIfPickA < worstScoreIfPickB 
                &&
                testA.getElementDirect( 0 ) > testB.getElementDirect( 0 ) ) {
                
                print( &testA, &testB );
                }
            else if( worstScoreIfPickB < worstScoreIfPickA
                     &&
                     testB.getElementDirect( 0 ) > 
                     testA.getElementDirect( 0 ) ) {
                
                print( &testA, &testB );
                }
            }
        else if( false && 
                 testA.getElementDirect( 0 ) == 
                 testB.getElementDirect( 0 ) ) {
            if( worstScoreIfPickA != worstScoreIfPickB ) {
                
                if( testA.getElementDirect( 1 ) == 
                    testB.getElementDirect( 1 ) &&
                    testA.getElementDirect( 2 ) == 
                    testB.getElementDirect( 2 ) &&
                    testA.getElementDirect( 3 ) == 
                    testB.getElementDirect( 3 )
                    ) {
                    

                    printf( "Worst if A: %d, Worst if B: %d\n",
                            worstScoreIfPickA, worstScoreIfPickB );
                    print( &testA, &testB );
                    }
                }
            }
        if( false ) {
            printf( "Worst if A: %d, Worst if B: %d\n",
                    worstScoreIfPickA, worstScoreIfPickB );
            print( &testA, &testB );
            }
        

        addOne( &testA, &testB );
        }
    }

