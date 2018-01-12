
#include "minorGems/io/file/File.h"
#include "minorGems/util/stringUtils.h"
#include "minorGems/util/MinPriorityQueue.h"

#include <stdio.h>
#include <time.h>



typedef struct Commit {
        time_t utcTime;
        struct tm localTime;
        int linesAdded;
    } Commit;

MinPriorityQueue<Commit> commitQueue;

SimpleVector<Commit> sortedList;






// handles where I was living
struct tm *myLocalTime( time_t inUTCTime ) {
    
    struct tm *tm = localtime( &inUTCTime );
    
    int year = tm->tm_year + 1900;
    

    // in Davis ?
    if( year > 2011 ) {
        return tm;
        }
    if( year == 2011 && tm->tm_mon >= 9 ) {
        // moved to Davis in october
        return tm;
        }
    
    // in Las Cruces?
    if( year > 2009 ) {
        inUTCTime += 3600;
        return localtime( &inUTCTime );
        }
    if( year == 2009 && tm->tm_mon >= 6 ) {
        // moved to LC in July
        inUTCTime += 3600;
        return localtime( &inUTCTime );
        }

    printf( "Potsdam\n" );
    // else in Potsdam, NY

    inUTCTime += 3600 * 3;
    return localtime( &inUTCTime );
    }



// started making games on June 10, 2004
char isInGameRange( time_t inUTCTime ) {
    struct tm *tm = myLocalTime( inUTCTime );
    
    if( tm->tm_year > 2004 ) {
        return true;
        }
    if( tm->tm_year < 2004 ) {
        return false;
        }
    if( tm->tm_mon < 5 ) {
        return false;
        }
    if( tm->tm_mon > 5 ) {
        return true;
        }
    if( tm->tm_mday >= 10 ) {
        return true;
        }
    return false;
    }







// portable version from man page from timegm
time_t my_timegm( struct tm *tm ){
    time_t ret;
    char *tz;
    
    tz = getenv("TZ");
    
    setenv("TZ", "", 1);
    tzset();
    ret = mktime(tm);
    if (tz)
        setenv("TZ", tz, 1);
    else
        unsetenv("TZ");
    tzset();
    return ret;
    }



// Wed Jan 10 10:59:23 2018 -0800
time_t parseHgGitTimeString( char *inTimeString ) {
    struct tm tm = {0};
    
    int utcOffsetSeconds = 0;
    
    // need to parse offset separately, because strptime doesn't do it correctly
    int numParts;
    char **parts = split( inTimeString, " ", &numParts );
    // index 5 is offset

    if( numParts > 5 ) {
        int offset;
        sscanf( parts[5], "%d", &offset );
        // this doesn't work for half-hour offsets, but don't worry about it
        int offsetHours = offset / 100;
        
        utcOffsetSeconds = offsetHours * 3600;
        //exit( 1 );
        }
    
    

    char *s = strptime( inTimeString, "%c %z", &tm );

    if( s != NULL ) {
        return my_timegm( &tm ) - utcOffsetSeconds;
        }
    else {
        printf( "Parsing hg/git time string '%s' failed\n", inTimeString );
        return time( NULL );
        }
    }


//2010/05/25 22:29:56
time_t parseCVSTimeString( char *inTimeString ) {
    struct tm tm = {0};
    
    char *s = strptime( inTimeString, "date: %Y/%m/%d %T", &tm );
    
    if( s != NULL ) {
        return my_timegm( &tm );
        }
    else {
        printf( "Parsing CVS time string '%s' failed\n", inTimeString );
        return time( NULL );
        }
    }


void addCommit( time_t inT, int inNumLines ) {
    if( isInGameRange( inT ) ) {
        struct tm *locTime = myLocalTime( inT );
        
        Commit c;
        c.utcTime = inT;
        c.localtime = *locTime;
        c.linesAdded = inNumLines;
        
        commitQueue.insert( c, difftime( inC.gmtTime, 0 ) );
        }
    }


void processGit( char **inLines, int inNumLines ) {
    for( int i=0; i<inNumLines; i++ ) {
        char *atLoc = strstr( inLines[i], "@" );
        char *insertionLoc = strstr( inLines[i], "insertion" );
        if( atLoc != NULL && insertionLoc != NULL ) {
            
            time_t t = parseHgGitTimeString( inLines[i] );
            
            char *commaLoc = strstr( atLoc, "," );
            
            int n = 0;
            
            sscanf( commaLoc, ", %d insertion", &n );
            
            addCommit( t, n );
            }
        }
    }

void processHg( char **inLines, int inNumLines ) {
    for( int i=0; i<inNumLines; i++ ) {
        
        char *plusLoc = strstr( inLines[i], "+" );
        if( plusLoc != NULL ) {
            time_t t = parseHgGitTimeString( inLines[i] );
            
            int n = 0;
            
            sscanf( plusLoc, "+%d", &n );
            
            addCommit( t, n );
            }
        }
    }


void processCVS( char **inLines, int inNumLines ) {
    for( int i=0; i<inNumLines; i++ ) {
        
        char *plusLoc = strstr( inLines[i], "+" );
        if( strstr( inLines[i], "date:" ) != NULL && plusLoc != NULL ) {
            time_t t = parseCVSTimeString( inLines[i] );
            
            int n = 0;
            
            sscanf( plusLoc, "+%d ", &n );
            
            addCommit( t, n );
            }
        }
    }




void processFile( File *inFile ) {
    
    if( ! inFile->isDirectory() ) {
        
        char *name = inFile->getFileName();
        
        char *contents = inFile->readFileContents();

        int numLines;
        char **lines = split( contents, "\n", &numLines );

        if( strstr( name, ".git" ) != NULL ) {
            printf( "Processing git log:  %s\n", name );
            processGit( lines, numLines );
            }
        else if( strstr( name, ".hg" ) != NULL ) {
            printf( "Processing hg log:  %s\n", name );
            processHg( lines, numLines );
            }
        if( strstr( name, ".cvs" ) != NULL ) {
            printf( "Processing cvs log:  %s\n", name );
            processCVS( lines, numLines );
            }
            
        for( int i=0; i<numLines; i++ ) {
            delete [] lines[i];
            }
        delete [] lines;

        delete [] contents;
        delete [] name;

        printf( "Growing commit list size = %d\n", commitQueue.size() );
        }
    }










void testTime() {
    
    // Wed Jan 10 10:59:23 2018 -0800

    time_t t = parseHgGitTimeString( "Fri Jan 12 10:51:56 2009 -0500" );
    

    //    time_t t = parseCVSTimeString( "date: 2006/11/26 20:32:23" );


    printf( "time = %u (string=%s)\n", (unsigned int)t, asctime( myLocalTime(t) ) );
    printf( "sec passed = %d\n", (int)( time(NULL) - t ) );
    
    exit(1);
    }






int main() {
    testTime();
    File dir( NULL, "." );
    
    if( dir.isDirectory() ) {
        
        int num;
        File **childFiles = dir.getChildFiles( &num );
        

        for( int i=0; i<num; i++ ) {
            processFile( childFiles[i] );
            delete childFiles[i];
            }
        delete [] childFiles;
        

        
        while( commitQueue.size() > 0 ) {
            sortedList.push_back( commitQueue.removeMin() );
            }
        }
    

    return 1;
    }
