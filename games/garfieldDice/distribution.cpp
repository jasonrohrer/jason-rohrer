#include <stdio.h>
#include <math.h>

int numDice = 9;


int facePayouts[7] = { 0, 1, 2, 3, 0, 0, 0 };
                       

// how many dice do we have left after getting payout from these faces?
int getOutcomeOfRoll( int *inDice, int inNumDice ) {
    char anyOneRolled = false;
    
    int numPayout = 0;
    
    for( int i=0; i<inNumDice; i++ ) {
        if( inDice[i] == 1 ) {
            anyOneRolled = true;
            }
        numPayout += facePayouts[ inDice[i] ];
        }
    
    if( anyOneRolled ) {
        //numPayout --;
        }
    return numPayout;
    }



int sumFaces( int *inDice, int inNumDice ) {
    int sum = 0;
    for( int i=0; i<inNumDice; i++ ) {
        sum += inDice[i];
        }
    return sum;
    }



int printDice( int *inDice, int inNumDice ) {
    for( int i=0; i<inNumDice; i++ ) {
        printf( "%d, ", inDice[i] );
        }
    printf( "\n" );
    }



int main( int inNumArgs, char **inArgs ) {

    int numOutconmes = numDice * 3 + 1;
    
    int *outcomeCounts = new int[ numOutconmes ];
    
    for( int i=0; i<numOutconmes; i++ ) {
        outcomeCounts[i] = 0;
        }
    
    int *dice = new int[ numDice ];
    
    for( int i=0; i<numDice; i++ ) {
        dice[i] = 1;
        }
    
    // keep counting up until all faces are 6
    while( sumFaces( dice, numDice ) < numDice * 6 ) {
        //printDice( dice, numDice );
        outcomeCounts[ getOutcomeOfRoll( dice, numDice ) ] ++;
        
        
        // add one and carry
        int next = 0;
        
        dice[next]++;
        
        while( dice[next] > 6 ) {
            dice[next] = 1;
            
            next ++;
            dice[next]++;
            }
        }
    

    // outcome of all 6 roll
    outcomeCounts[ getOutcomeOfRoll( dice, numDice ) ] ++;

    int outcomeCount = 0;
    int outcomeSum = 0;
    
    for( int i=0; i<numOutconmes; i++ ) {
        outcomeCount += outcomeCounts[i];
        outcomeSum += i * outcomeCounts[i];
        }

    for( int i=0; i<numOutconmes; i++ ) {
        printf( "%d %f\n", i, outcomeCounts[i] / (double)outcomeSum );
        }
    
    
    
    double meanOutcome = outcomeSum / (double)outcomeCount;
    
    printf( "Count = %d\n", outcomeCount );
    printf( "Sum = %d\n", outcomeSum );
    printf( "Mean = %f\n", meanOutcome );

    
    int stdDevSum = 0;
    
    for( int i=0; i<numOutconmes; i++ ) {
        int diff = i - meanOutcome;
        
        int diff2 = diff * diff;
        
        stdDevSum += diff2 * outcomeCounts[i];
        }
    
    double stdDev = sqrt( stdDevSum / (double)outcomeCount );
    
    printf( "Std Dev = %f\n", stdDev );
    }
