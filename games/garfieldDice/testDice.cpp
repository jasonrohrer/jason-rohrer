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
    
    int houseBalance = 1000;
    

    int dieBuy = 20;
    int dieSell = 18;
    
    int playerDiceBuy = 5;

    //double cashOutChance = 0.05;
    double cashOutChance = -0.01;

    int lowBalance = houseBalance;
    int highBalance = houseBalance;
    

    for( int i=0; i<numPlayers; i++ ) {

        int roundCount = 0;
        
        int playerDice = playerDiceBuy;

        int playerBuyIn = playerDiceBuy * dieBuy; 
        
        houseBalance += playerBuyIn;
        
        char cashOut = false;
        
        while( ! cashOut && playerDice > 0 ) {
            roundCount ++;
            
            int newPlayerDice = 0;
            for( int d=0; d<playerDice; d++ ) {
                int roll = randSource.getRandomBoundedInt( 0, 5 );
                
                newPlayerDice += earnDie[ roll ];
                }
            
            playerDice = newPlayerDice;
            
            double cashOutRoll = randSource.getRandomBoundedDouble( 0, 1 );
            
            if( cashOutRoll <= cashOutChance ) {
                cashOut = true;
                }
            if( playerDice > 100 ) {
                cashOut = true;
                }
            }

        int playerLeftWith = playerDice * dieSell; 

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
