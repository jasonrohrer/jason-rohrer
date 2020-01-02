#include <stdio.h>
#include <stdlib.h>

#include "minorGems/util/random/JenkinsRandomSource.h"



int main( int inNumArgs, char **inArgs ) {
    
    int numPlayers = 100;
    

    if( inNumArgs > 1 ) {
        sscanf( inArgs[1], "%d", &numPlayers );
        }
    

    JenkinsRandomSource randSource;
    

    int earnDie[6] = { 1, 2, 3, 0, 0, 0 };
    //int earnDie[6] = { -1, 3, -3, 5, -5, 7 };
    //int earnDie[6] = { 0, 0, 0, 0, 5, 0 };
    
    int houseBalance = 1000;
    

    int dieBuy = 1;
    int dieSell = 1;
    
    int playerDiceBuy = 6;

    double cashOutChance = 0.50;

    int cashOutConsider = 12;
    

    int lowBalance = houseBalance;
    int highBalance = houseBalance;

    int goldenDiceThreshold = -1;
    
    int goldenDieSell = dieBuy;
    
    int maxDice = 5000;

    #define NUM_BINS 200
    int bins[NUM_BINS];
    for( int i=0; i<NUM_BINS; i++ ) {
        bins[i] = 0;
        }


    for( int i=0; i<numPlayers; i++ ) {

        int roundCount = 0;
        
        int playerDice = playerDiceBuy;

        int playerBuyIn = playerDiceBuy * dieBuy; 
        
        int playerGoldenDice = 0;
        

        houseBalance += playerBuyIn;
        
        char cashOut = false;
        
        while( ! cashOut && playerDice > 0 ) {
            roundCount ++;

            int newPlayerGoldenDice = 0;
            for( int d=0; d<playerGoldenDice; d++ ) {
                int roll = randSource.getRandomBoundedInt( 0, 5 );
                
                newPlayerGoldenDice += earnDie[ roll ];
                }
            
            playerGoldenDice = newPlayerGoldenDice;

            
            int newPlayerDice = 0;
            char anyOnes = false;
            for( int d=0; d<playerDice; d++ ) {
                int roll = randSource.getRandomBoundedInt( 0, 5 );
                
                newPlayerDice += earnDie[ roll ];
                if( roll == 0 ) {
                    anyOnes = true;
                    }
                }
            
            // rolling any 1 is penalty of losing one of those 1 dice
            if( anyOnes ) {
                newPlayerDice -= 1;
                }

            
            playerDice = newPlayerDice;

            if( goldenDiceThreshold != -1 && 
                playerDice > goldenDiceThreshold ) {
                playerGoldenDice += playerDice - goldenDiceThreshold;
                playerDice = goldenDiceThreshold;
                }

            if( playerDice > cashOutConsider || playerGoldenDice > 0 ) {
                
                double cashOutRoll = randSource.getRandomBoundedDouble( 0, 1 );
                
                if( cashOutRoll <= cashOutChance ) {
                    cashOut = true;
                    }
                }
            if( playerDice >= maxDice ) {
                cashOut = true;
                }
            }

        int playerLeftWith = 
            playerDice * dieSell + 
            playerGoldenDice * goldenDieSell; 

        houseBalance -= playerLeftWith;
        
        if( false ) printf( "Player bount in for %d, played %d rounds, "
                "left with %d, house balance = %d\n",
                playerBuyIn, roundCount, playerLeftWith, houseBalance );

        if( houseBalance < lowBalance ) {
            lowBalance = houseBalance;
            if( false )printf( "New low balance in round %d:  %d\n", i, lowBalance );
            }
        if( houseBalance > highBalance ) {
            highBalance = houseBalance;
            if( false )printf( "New hight balance in round %d:  %d\n", i, highBalance );
            }
        printf( "%d %d %d\n", i, houseBalance, roundCount );
        
        if( roundCount < NUM_BINS ) {
            bins[ roundCount ] ++;
            }
        else {
            bins[ NUM_BINS - 1 ] ++;
            }
        }

    FILE *binFile = fopen( "bins4.txt", "w" );
    
    for( int i=0; i<NUM_BINS; i++ ) {
        fprintf( binFile, "%d %d\n", i, bins[i] );
        }
    fclose( binFile );
    }
