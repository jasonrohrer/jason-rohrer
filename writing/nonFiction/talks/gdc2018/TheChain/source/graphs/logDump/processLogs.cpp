
#include "minorGems/io/file/File.h"
#include "minorGems/util/stringUtils.h"
#include "minorGems/util/MinPriorityQueue.h"

#include <stdio.h>
#include <time.h>



typedef struct Commit {
        time_t utcTime;
        struct tm localTime;
        int linesAdded;
        
        char *rawLogLine;
        char *logName;
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

    // else in Potsdam, NY

    inUTCTime += 3600 * 3;
    return localtime( &inUTCTime );
    }



// started making games on June 10, 2004
char isInGameRange( time_t inUTCTime ) {
    struct tm *tm = myLocalTime( inUTCTime );

    int year = tm->tm_year + 1900;
    
    if( year > 2004 ) {
        return true;
        }
    if( year < 2004 ) {
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
    
    

    char *s = strptime( inTimeString, "%c", &tm );

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


double currentPriority = 0;

// commits that all happened at the same time
SimpleVector<Commit> currentCommitBin;


void addCommit( time_t inT, int inNumLines, char *inLogLine, char *inName ) {
    if( isInGameRange( inT ) ) {

        double priority = difftime( inT, 0 );

            
        struct tm *locTime = myLocalTime( inT );
        
        Commit c;
        c.utcTime = inT;
        c.localTime = *locTime;
        c.linesAdded = inNumLines;
        c.rawLogLine = stringDuplicate( inLogLine );
        c.logName = stringDuplicate( inName );
        
        commitQueue.insert( c, priority );
        }
    }


void processGit( char **inLines, int inNumLines, char *inName ) {
    for( int i=0; i<inNumLines; i++ ) {
        char *atLoc = strstr( inLines[i], "@" );
        char *insertionLoc = strstr( inLines[i], "insertion" );
        if( atLoc != NULL && insertionLoc != NULL ) {
            
            time_t t = parseHgGitTimeString( inLines[i] );
            
            char *commaLoc = strstr( atLoc, "," );
            
            int n = 0;
            
            sscanf( commaLoc, ", %d insertion", &n );
            
            addCommit( t, n, inLines[i], inName );
            }
        }
    }

void processHg( char **inLines, int inNumLines, char *inName ) {
    for( int i=0; i<inNumLines; i++ ) {
        
        char *plusLoc = strstr( inLines[i], "+" );
        if( plusLoc != NULL ) {
            time_t t = parseHgGitTimeString( inLines[i] );
            
            int n = 0;
            
            sscanf( plusLoc, "+%d", &n );
            
            addCommit( t, n, inLines[i], inName );
            }
        }
    }


void processCVS( char **inLines, int inNumLines, char *inName ) {
    for( int i=0; i<inNumLines; i++ ) {
        
        char *plusLoc = strstr( inLines[i], "+" );
        if( strstr( inLines[i], "date:" ) != NULL && plusLoc != NULL ) {
            time_t t = parseCVSTimeString( inLines[i] );
            
            int n = 0;
            
            sscanf( plusLoc, "+%d ", &n );
            if( n > 1000 ) {
                printf( "Huge commit:  %s\n", inLines[i] );
                }
            addCommit( t, n, inLines[i], inName );
            }
        }
    }




void processFile( File *inFile ) {
    
    if( ! inFile->isDirectory() ) {
        
        char *name = inFile->getFileName();
        
        if( strstr( name, ".git" ) != NULL ||
            strstr( name, ".hg" ) != NULL ||
            strstr( name, ".cvs" ) != NULL ) {
            

            char *contents = inFile->readFileContents();
            
            int numLines;
            char **lines = split( contents, "\n", &numLines );
            
            if( strstr( name, ".git" ) != NULL ) {
                printf( "Processing git log:  %s\n", name );
                processGit( lines, numLines, name );
                }
            else if( strstr( name, ".hg" ) != NULL ) {
                printf( "Processing hg log:  %s\n", name );
                processHg( lines, numLines, name );
                }
            if( strstr( name, ".cvs" ) != NULL ) {
                printf( "Processing cvs log:  %s\n", name );
                processCVS( lines, numLines, name );
                }
            
            for( int i=0; i<numLines; i++ ) {
                delete [] lines[i];
                }
            delete [] lines;

            delete [] contents;
            }
        
        delete [] name;

        printf( "Growing commit list size = %d\n", commitQueue.size() );
        }
    }










void testTime() {
    
    // Wed Jan 10 10:59:23 2018 -0800

    time_t t = parseHgGitTimeString( (char*)"Fri Jan 12 10:51:56 2009 -0500" );
    

    //    time_t t = parseCVSTimeString( "date: 2006/11/26 20:32:23" );


    printf( "time = %u (string=%s)\n", 
            (unsigned int)t, asctime( myLocalTime(t) ) );
    printf( "sec passed = %d\n", (int)( time(NULL) - t ) );
    
    exit(1);
    }



typedef struct TimeBin {
        time_t binStartTime;
        int numLines;
    } TimeBin;
        


    
void countBinnedLines( const char *inStartTimeString,
                       int inSecPerBin,
                       FILE *inOutputFile ) {
    

    struct tm tm;
    
    // first week to consider
    strptime( inStartTimeString, "%c", &tm );
    
    // of current week
    time_t startTime = my_timegm( &tm );

    
    
    time_t endTime = startTime + inSecPerBin;
    
    int index = 0;
    
    SimpleVector<TimeBin> bins;
    

    while( index < sortedList.size() ) {
        
        TimeBin b = { startTime, 0 };
                    
        char binOfInterest = false;
        
        time_t t = startTime;
        
        struct tm *binTM = localtime( &startTime );
        
        if( binTM->tm_mon == 7 && 
            binTM->tm_year + 1900 == 2004 &&
            binTM->tm_mday == 28 ) {
            binOfInterest = true;
            }
  
        
        Commit *c = sortedList.getElement( index );
        
        while( index < sortedList.size() && 
               c->utcTime >= startTime && 
               c->utcTime < endTime ) {
            
            // commit in this week
            
            // any single commits with more than 500 lines aren't handwritten
            // code (code imports, boilerplate, text databases, etc).
            if( c->linesAdded < 500 ) {    
                b.numLines += c->linesAdded;
                
                if( binOfInterest ) {
                    printf( "adding %d lines to 8/28/04, new total %d\n",
                            c->linesAdded, b.numLines );
                    }
                }
            
            index++;
            if( index < sortedList.size() ) {
                c = sortedList.getElement( index );
                }
            }
        
        bins.push_back( b );
        
        startTime = endTime;
        endTime = startTime + inSecPerBin;
        }
    


    for( int i=0; i<bins.size(); i++ ) {
        char buff[100];
        TimeBin *b = bins.getElement(i);
        
        time_t t = b->binStartTime;
        
        strftime( buff, 100, "%x", localtime( &t ) );
        fprintf( inOutputFile, "%s %d\n", buff, b->numLines );
        }
    }


void countWeeklyLines() {
    const char *outName = "../locPerWeek_time.dat";
    
    FILE *outFile = fopen( outName, "w" );
    
    if( outFile == NULL ) {
        printf( "Failed to open output file %s\n", outName );
        return;
        }
    int weekSec = 3600 * 24 * 7;
    countBinnedLines( "Sun Jun 06 00:00:00 2004",
                      weekSec,
                      outFile );
    fclose( outFile );
    }


void countMonthlyLines() {
    const char *outName = "../locPerMonth_time.dat";
    
    FILE *outFile = fopen( outName, "w" );
    
    if( outFile == NULL ) {
        printf( "Failed to open output file %s\n", outName );
        return;
        }
    int monthSec = 2628000;
    countBinnedLines( "Tue Jun 01 00:00:00 2004",
                      monthSec,
                      outFile );
    fclose( outFile );
    }





int main() {
    //testTime();
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
            double priority = commitQueue.checkMinPriority();
            Commit c = commitQueue.removeMin();

            if( priority != currentPriority ) {
                currentCommitBin.deleteAll();
                currentPriority = priority;
                }
        
            // make sure we haven't already seen a commit at this time
            // with this exact number of lines
            // CVS logs contain repeats, it seems
            // and Transcend log contains duplicates of some minorGems logs

            char found = false;
            for( int i=0; i<currentCommitBin.size(); i++ ) {
                if( currentCommitBin.getElement( i )->linesAdded == 
                    c.linesAdded ) {
                    found = true;
                    break;
                    }
                }
        

            if( ! found ) {
                sortedList.push_back( c );
                currentCommitBin.push_back( c );
                }
            else {
                delete [] c.rawLogLine;
                delete [] c.logName;
                }
            
            }
        
        countWeeklyLines();
        countMonthlyLines();
        }
    
    
    FILE *allLogsOut = fopen( "allLogs.txt", "w" );
    
    for( int i=0; i<sortedList.size(); i++ ) {
        if( allLogsOut != NULL ) {
            fprintf( allLogsOut, "%s %s\n", 
                     sortedList.getElement( i )->rawLogLine,
                     sortedList.getElement( i )->logName );
            }
        delete [] sortedList.getElement( i )->rawLogLine;
        delete [] sortedList.getElement( i )->logName;
        }
    
    if( allLogsOut != NULL ) {    
        fclose( allLogsOut );
        }
    


    return 1;
    }
