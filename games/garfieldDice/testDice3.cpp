#include <stdio.h>
#include <stdlib.h>

#include "minorGems/util/random/JenkinsRandomSource.h"
#include "minorGems/util/SimpleVector.h"


void stratRerollAll( SimpleVector<int> *inVal, 
                     SimpleVector<char> *inRerollFlags ) {
    for( int i=0; i<inRerollFlags->size(); i++ ) {
        *( inRerollFlags->getElement(i) ) = true;
        }
    }


void stratKeepSpecific( int inValToKeep,
                        SimpleVector<int> *inVal, 
                        SimpleVector<char> *inRerollFlags ) {
    int indexToKeep = inValToKeep - 1;
    
    for( int i=0; i<inRerollFlags->size(); i++ ) {
        if( inVal->getElementDirect( i ) != indexToKeep ) {
            *( inRerollFlags->getElement(i) ) = true;
            }
        }
    }



void stratKeepOneSpecific( int inValToKeep,
                        SimpleVector<int> *inVal, 
                        SimpleVector<char> *inRerollFlags ) {
    int indexToKeep = inValToKeep - 1;
    
    char found = false;
    
    for( int i=0; i<inRerollFlags->size(); i++ ) {
        if( inVal->getElementDirect( i ) != indexToKeep ) {
            *( inRerollFlags->getElement(i) ) = true;
            }
        else if( ! found ) {
            found = true;
            }
        else {
            // only keep first found
            *( inRerollFlags->getElement(i) ) = true;
            }
        }
    }


void stratDiscardSpecific( int inValToDiscard,
                        SimpleVector<int> *inVal, 
                        SimpleVector<char> *inRerollFlags ) {
    int indexToDiscard = inValToDiscard - 1;
    
    for( int i=0; i<inRerollFlags->size(); i++ ) {
        if( inVal->getElementDirect( i ) == indexToDiscard ) {
            *( inRerollFlags->getElement(i) ) = true;
            }
        }
    }



void stratDiversify( SimpleVector<int> *inVal, 
                     SimpleVector<char> *inRerollFlags ) {
    
    int count[6] = { 0,0,0, 0,0,0 };

    for( int i=0; i<inRerollFlags->size(); i++ ) {
        
        count[ inVal->getElementDirect( i ) ] ++;
        
        if( count[ inVal->getElementDirect( i ) ] > 1 ) {
            *( inRerollFlags->getElement(i) ) = true;
            }
        }
    }






int main( int inNumArgs, char **inArgs ) {
    
    int numPlayers = 100;
    

    if( inNumArgs > 1 ) {
        sscanf( inArgs[1], "%d", &numPlayers );
        }
    

    JenkinsRandomSource randSource( time( NULL ) + 3 );
    

    //int earnDie[6] = { 1, 2, 3, 0, 0, 0 };
    //int earnDie[6] = { -1, 3, -3, 5, -5, 7 };
    // int earnDie[6] = { 0, 0, 0, 0, 5, 0 };

    // can have 3 kill dice with this multiplication factor
    //int earnDie[6] = { 2, 3, 4, 5, 6, 7 };
    
    // or 2 kill dice with this multiplication factor
    int earnDie[6] = { 1, 2, 3, 4, 5, 6 };
    //int earnDie[6] = { 2, 3, 4, 5, 6, 7 };
    
    
    int houseBalance = 1000;
    

    int dieBuy = 2;
    int dieSell = 2;
    
    int playerDiceBuy = 12;

    double cashOutChance = 0.10;

    int cashOutConsider = 11;
    
    int maxDice = 1000;
    

    int lowBalance = houseBalance;
    int highBalance = houseBalance;

    

    for( int i=0; i<numPlayers; i++ ) {

        int roundCount = 0;
        
        int playerDice = playerDiceBuy;

        int playerBuyIn = playerDiceBuy * dieBuy; 
        

        SimpleVector<int> playerDiceValues;
        
        SimpleVector<char> playerDiceRerollFlags;

        // must reroll all on first roll
        for( int d=0; d<playerDice; d++ ) {
            playerDiceValues.push_back( 1 );
            playerDiceRerollFlags.push_back( true );
            }


        houseBalance += playerBuyIn;
        
        char cashOut = false;
        
        while( ! cashOut && playerDice > 0 ) {
            roundCount ++;

            int specialDieA = randSource.getRandomBoundedInt( 0, 5 );

            int specialDieB = randSource.getRandomBoundedInt( 0, 5 );
            int specialDieC = randSource.getRandomBoundedInt( 0, 5 );
            int specialDieD = randSource.getRandomBoundedInt( 0, 5 );
            int specialDieE = randSource.getRandomBoundedInt( 0, 5 );
            int specialDieF = randSource.getRandomBoundedInt( 0, 5 );


            int specialDieT = randSource.getRandomBoundedInt( 0, 5 );
            
            
            // trap wolves
            if( specialDieT == specialDieB ) {
                specialDieB = -1;
                }
            if( specialDieT == specialDieC ) {
                specialDieC = -1;
                }
            if( specialDieT == specialDieD ) {
                specialDieD = -1;
                }
            

            int newPlayerDice = 0;

            SimpleVector<int> newPlayerDiceValues;
            SimpleVector<char> newPlayerDiceRerollFlags;
            
            for( int d=0; d<playerDice; d++ ) {

                int roll = randSource.getRandomBoundedInt( 0, 5 );
                
                if( playerDiceRerollFlags.getElementDirect( d ) == false ) {
                    roll = playerDiceValues.getElementDirect( d );
                    }

                if( roll == specialDieB 
                    || roll == specialDieC 
                    || roll == specialDieD 
                    //|| roll == specialDieD 
                    // || roll == specialDieF 
                    ) {
                    // hit by kill die, croaks
                    }
                else if( roll == specialDieA ) {
                    // only special die earns new dice
                    // all others croak
                    newPlayerDice += earnDie[ roll ];
                    
                    newPlayerDiceValues.push_back( roll );
                    newPlayerDiceRerollFlags.push_back( false );
                    
                    // gained dice MUST be rerolled
                    for( int e=0; e<earnDie[ roll ]; e++ ) {
                        newPlayerDiceValues.push_back( roll );
                        newPlayerDiceRerollFlags.push_back( true );
                        }
                    }
                else {
                    // just survives
                    newPlayerDice ++;
                    newPlayerDiceValues.push_back( roll );
                    newPlayerDiceRerollFlags.push_back( false );
                    }
                }
            
            playerDice = newPlayerDice;

            playerDiceValues = newPlayerDiceValues;
            playerDiceRerollFlags = newPlayerDiceRerollFlags;
            

            if( playerDice > cashOutConsider ) {
                
                double cashOutRoll = randSource.getRandomBoundedDouble( 0, 1 );
                
                if( cashOutRoll <= cashOutChance ) {
                    cashOut = true;
                    }
                }
            if( playerDice >= maxDice ) {
                cashOut = true;
                }


            // now execute reroll strategy
            //stratRerollAll( &playerDiceValues, &playerDiceRerollFlags );
            //            stratKeepOneSpecific( 6, &playerDiceValues, &playerDiceRerollFlags );
            //stratKeepSixes( &playerDiceValues, &playerDiceRerollFlags );
            stratKeepSpecific( 6, &playerDiceValues, &playerDiceRerollFlags );
            //stratKeepOneSpecific( 6, &playerDiceValues, &playerDiceRerollFlags );
            //stratDiscardSpecific( 1, &playerDiceValues, &playerDiceRerollFlags );
            //stratDiscardSpecific( 2, &playerDiceValues, &playerDiceRerollFlags );
            //stratDiscardSpecific( 3, &playerDiceValues, &playerDiceRerollFlags );
            //stratDiscardSpecific( 4, &playerDiceValues, &playerDiceRerollFlags );
            //stratDiversify( &playerDiceValues, &playerDiceRerollFlags );
            //stratDiscardSpecific( 
            //    2, &playerDiceValues, &playerDiceRerollFlags );
            }

        int playerLeftWith = 
            playerDice * dieSell; 

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
