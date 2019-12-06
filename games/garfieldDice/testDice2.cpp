#include <stdio.h>
#include <stdlib.h>

#include "minorGems/util/random/JenkinsRandomSource.h"



int main( int inNumArgs, char **inArgs ) {
    
    int numPlayers = 100;
    

    if( inNumArgs > 1 ) {
        sscanf( inArgs[1], "%d", &numPlayers );
        }
    

    JenkinsRandomSource randSource;
    

    //int earnDie[6] = { 1, 2, 3, 0, 0, 0 };
    //int earnDie[6] = { -1, 3, -3, 5, -5, 7 };
    // int earnDie[6] = { 0, 0, 0, 0, 5, 0 };
    int earnDie[6] = { 2, 3, 4, 5, 6, 7 };
    //int earnDie[6] = { 1, 2, 3, 4, 5, 6 };
    
    int houseBalance = 1000;
    

    int dieBuy = 2;
    int dieSell = 2;
    
    int playerDiceBuy = 12;

    double cashOutChance = 0.10;

    int cashOutConsider = 11;
    

    int lowBalance = houseBalance;
    int highBalance = houseBalance;

    int goldenDiceThreshold = -1;
    
    int goldenDieSell = dieBuy;
    

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


            int specialDie = randSource.getRandomBoundedInt( 0, 5 );
            
            int newPlayerDice = 0;
            for( int d=0; d<playerDice; d++ ) {

                // only roll first die if unfavorable
                // else roll all dice

                // actually, can't get this to work with structure where
                // each special die state earns at least one die
                // (very nice to have die face tell you how many extra die
                //  you win)
                // SO... to make this work and be profitable, players have
                // to pick how many dice they roll BEFORE they know what
                // the special die is (or roll it at the same time as their
                // chosen die)
                // This removes a skill from the game, but it was a false
                // skill anyway, because no player would commit dice after the
                // special die shows a house edge.
                if( true || specialDie >= 3 || d == 0 ) {
                    
                    int roll = randSource.getRandomBoundedInt( 0, 5 );
                    
                    if( roll == specialDie ) {
                        // only special die earns new dice
                        // all others croak
                        newPlayerDice += earnDie[ roll ];
                        }
                    }
                else {
                    // keep die and don't roll it at all
                    newPlayerDice ++;
                    }
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
        printf( "%d %d\n", i, houseBalance );
        }
    }
