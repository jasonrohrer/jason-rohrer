
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

        char considerFlag;
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



typedef struct LOCBin {
        time_t binStartTime;
        int numLinesIn;
        int numLinesOut;
    } LOCBin;
        


    
void countBinnedLines( const char *inStartTimeString,
                       const char *inEndTimeString,
                       int inSecPerBin,
                       FILE *inOutputFile ) {
    

    struct tm tm;
    
    // first week to consider
    strptime( inStartTimeString, "%c", &tm );
    
    // of current week
    time_t startTime = my_timegm( &tm );

    // cut-off time
    strptime( inEndTimeString, "%c", &tm );    
    time_t finalEndTime = my_timegm( &tm );

    
    
    time_t endTime = startTime + inSecPerBin;
    
    int index = 0;
    
    SimpleVector<LOCBin> bins;
    
    // skip any that are before first start time
    while( index < sortedList.size() &&
           sortedList.getElement( index )->utcTime < startTime ) {
        index ++;
        }


    while( index < sortedList.size() && endTime < finalEndTime ) {
        
        LOCBin b = { startTime, 0 };
                    
        
        Commit *c = sortedList.getElement( index );
        
        while( index < sortedList.size() && 
               c->utcTime >= startTime && 
               c->utcTime < endTime ) {
            
            // commit in this time bin
            
            if( c->considerFlag ) {
                // and this commit is flagged as under consideration
                b.numLinesIn += c->linesAdded;
                }
            else {
                // also track total of commits NOT considered
                b.numLinesOut += c->linesAdded;
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
        LOCBin *b = bins.getElement(i);
        
        time_t t = b->binStartTime;
        
        strftime( buff, 100, "%x", localtime( &t ) );

        int total = b->numLinesIn + b->numLinesOut;
        
        double fraction = 1.0;
        
        if( total > 0 ) {
            fraction = b->numLinesIn / (double)( total );
            }
        
        fprintf( inOutputFile, "%s %d %f\n", buff, b->numLinesIn, fraction );
        }
    }


void countWeeklyLines() {
    for( int i=0; i<sortedList.size(); i++ ) {
        sortedList.getElement( i )->considerFlag = true;
        }
    
    const char *outName = "../locPerWeek_time.dat";
    
    FILE *outFile = fopen( outName, "w" );
    
    if( outFile == NULL ) {
        printf( "Failed to open output file %s\n", outName );
        return;
        }
    int weekSec = 3600 * 24 * 7;
    countBinnedLines( "Sun Jun 06 00:00:00 2004",
                      "Sun Jun 06 00:00:00 2019",
                      weekSec,
                      outFile );
    fclose( outFile );
    }



void countWeeklyLinesOHOL() {
    for( int i=0; i<sortedList.size(); i++ ) {
        sortedList.getElement( i )->considerFlag = true;
        }
    
    const char *outName = "../locPerWeek_ohol.dat";
    
    FILE *outFile = fopen( outName, "w" );
    
    if( outFile == NULL ) {
        printf( "Failed to open output file %s\n", outName );
        return;
        }
    int weekSec = 3600 * 24 * 7;
    countBinnedLines( "Thu May 28 00:00:00 2015",
                      "Sun Jun 06 00:00:00 2019",
                      weekSec,
                      outFile );
    fclose( outFile );
    }



void countMonthlyLines() {
    for( int i=0; i<sortedList.size(); i++ ) {
        sortedList.getElement( i )->considerFlag = true;
        }
    
    const char *outName = "../locPerMonth_time.dat";
    
    FILE *outFile = fopen( outName, "w" );
    
    if( outFile == NULL ) {
        printf( "Failed to open output file %s\n", outName );
        return;
        }
    int monthSec = lrint( 365.25 * 24 * 3600 ) / 12;
    countBinnedLines( "Tue Jun 01 00:00:00 2004",
                      "Sun Jun 06 00:00:00 2019",
                      monthSec,
                      outFile );
    fclose( outFile );
    }



void countYearlyLines() {
    for( int i=0; i<sortedList.size(); i++ ) {
        sortedList.getElement( i )->considerFlag = true;
        }
    
    const char *outName = "../locPerYear_time.dat";
    
    FILE *outFile = fopen( outName, "w" );
    
    if( outFile == NULL ) {
        printf( "Failed to open output file %s\n", outName );
        return;
        }
    int yearSec = lrint( 365.25 *24*3600 );
    countBinnedLines( "Thu Jan 01 00:00:00 2004",
                      "Sun Jun 06 00:00:00 2019",
                      yearSec,
                      outFile );
    fclose( outFile );
    }




void countDayOfWeekLines( const char *inStartTimeString, 
                          const char *inEndTimeString,
                          const char *inOutFileName ) {

    struct tm tm;
    
    strptime( inStartTimeString, "%c", &tm );
    
    // of current week
    time_t startTime = my_timegm( &tm );

    
    // cut-off time
    strptime( inEndTimeString, "%c", &tm );    
    time_t finalEndTime = my_timegm( &tm );


    int bins[7];
    for( int i=0; i<7; i++ ) {
        bins[i] = 0;
        }
    
    const char *binNames[7] = { "Sunday", "Monday",
                                "Tuesday", "Wednesday", "Thursday",
                                "Friday", "Saturday" };
    
    int total = 0;
    
    for( int i=0; i<sortedList.size(); i++ ) {
        Commit *c = sortedList.getElement( i );
        
        if( c->utcTime >= startTime && c->utcTime < finalEndTime ) {
            bins[ c->localTime.tm_wday ] += c->linesAdded;
            total += c->linesAdded;
            }
        }
    const char *outName = inOutFileName;
    
    FILE *outFile = fopen( outName, "w" );

    if( outFile != NULL ) {
        for( int i=0; i<7; i++ ) {
            fprintf( outFile, "%d %s %d %f\n",
                     i, binNames[i], bins[i], (double)( bins[i] ) / total );
            }
        fclose( outFile );
        }
    
    }




void countHourOfDayLines( const char *inStartTimeString, 
                          const char *inEndTimeString,
                          const char *inOutFileName ) {
    struct tm tm;

    strptime( inStartTimeString, "%c", &tm );
    
    // of current week
    time_t startTime = my_timegm( &tm );

    
    // cut-off time
    strptime( inEndTimeString, "%c", &tm );    
    time_t finalEndTime = my_timegm( &tm );


    int bins[24];
    for( int i=0; i<24; i++ ) {
        bins[i] = 0;
        }
    
    char *binNames[24];
    
    binNames[0] = stringDuplicate( "Midnight" );
    binNames[12] = stringDuplicate( "Noon" );
    
    for( int i=0; i<24; i++ ) {
        if( i != 0 && i != 12 ) {
            if( i < 12 ) {
                binNames[i] = autoSprintf( "%d AM", i );
                }
            else {
                binNames[i] = autoSprintf( "%d PM", i - 12 );
                }
            }
        }
    

    int total = 0;

    for( int i=0; i<sortedList.size(); i++ ) {
        Commit *c = sortedList.getElement( i );
        
        if( c->utcTime >= startTime && c->utcTime < finalEndTime ) {
            bins[ c->localTime.tm_hour ] += c->linesAdded;
            total += c->linesAdded;
            }
        }

    const char *outName = inOutFileName;
    
    FILE *outFile = fopen( outName, "w" );

    if( outFile != NULL ) {
        for( int i=0; i<24; i++ ) {
            fprintf( outFile, "%d \"%s\" %d %f\n",
                     i, binNames[i], bins[i],
                     (double)( bins[i] ) / total );
            }
        fclose( outFile );
        }

    for( int i=0; i<24; i++ ) {
        delete [] binNames[i];
        }
    }




void countYearlyWednesdayLines() {
    for( int i=0; i<sortedList.size(); i++ ) {
        Commit *c = sortedList.getElement( i );
        c->considerFlag = 
            ( c->localTime.tm_wday == 3 );
        }
    
    const char *outName = "../locPerYearWednesday.dat";
    
    FILE *outFile = fopen( outName, "w" );
    
    if( outFile == NULL ) {
        printf( "Failed to open output file %s\n", outName );
        return;
        }
    int yearSec = lrint( 365.25 *24*3600 );
    countBinnedLines( "Thu Jan 01 00:00:00 2004",
                      "Sun Jun 06 00:00:00 2019",
                      yearSec,
                      outFile );
    fclose( outFile );
    }



void countYearlyFridayLines() {
    for( int i=0; i<sortedList.size(); i++ ) {
        Commit *c = sortedList.getElement( i );
        c->considerFlag = 
            ( c->localTime.tm_wday == 5 );
        }
    
    const char *outName = "../locPerYearFriday.dat";
    
    FILE *outFile = fopen( outName, "w" );
    
    if( outFile == NULL ) {
        printf( "Failed to open output file %s\n", outName );
        return;
        }
    int yearSec = lrint( 365.25 *24*3600 );
    countBinnedLines( "Thu Jan 01 00:00:00 2004",
                      "Sun Jun 06 00:00:00 2019",
                      yearSec,
                      outFile );
    fclose( outFile );
    }



void countYearlyOnePMLines() {
    for( int i=0; i<sortedList.size(); i++ ) {
        Commit *c = sortedList.getElement( i );
        c->considerFlag = 
            ( c->localTime.tm_hour == 13 );
        }
    
    const char *outName = "../locPerYearOnePM.dat";
    
    FILE *outFile = fopen( outName, "w" );
    
    if( outFile == NULL ) {
        printf( "Failed to open output file %s\n", outName );
        return;
        }
    int yearSec = lrint( 365.25 *24*3600 );
    countBinnedLines( "Thu Jan 01 00:00:00 2004",
                      "Sun Jun 06 00:00:00 2019",
                      yearSec,
                      outFile );
    fclose( outFile );
    }



void countYearlyNineAMLines() {
    for( int i=0; i<sortedList.size(); i++ ) {
        Commit *c = sortedList.getElement( i );
        c->considerFlag = 
            ( c->localTime.tm_hour == 9 );
        }
    
    const char *outName = "../locPerYearNineAM.dat";
    
    FILE *outFile = fopen( outName, "w" );
    
    if( outFile == NULL ) {
        printf( "Failed to open output file %s\n", outName );
        return;
        }
    int yearSec = lrint( 365.25 *24*3600 );
    countBinnedLines( "Thu Jan 01 00:00:00 2004",
                      "Sun Jun 06 00:00:00 2019",
                      yearSec,
                      outFile );
    fclose( outFile );
    }








typedef struct HourBin {
        time_t binStartTime;
        int numHoursIn;
        int numHoursOut;
    } HourBin;
        


    
void countBinnedHours( const char *inStartTimeString,
                       const char *inEndTimeString,
                       int inSecPerBin,
                       FILE *inOutputFile ) {
    

    struct tm tm;
    
    // first week to consider
    strptime( inStartTimeString, "%c", &tm );
    
    // of current week
    time_t startTime = my_timegm( &tm );

    
    // cut-off time
    strptime( inEndTimeString, "%c", &tm );    
    time_t finalEndTime = my_timegm( &tm );


    
    
    time_t endTime = startTime + inSecPerBin;
    
    int index = 0;
    
    SimpleVector<HourBin> bins;
    
    // skip any that are before first start time
    while( index < sortedList.size() &&
           sortedList.getElement( index )->utcTime < startTime ) {
        index ++;
        }

    struct tm lastCounted;
    lastCounted.tm_hour = 0;
    lastCounted.tm_year = 0;
    lastCounted.tm_yday = 1;

    while( index < sortedList.size() && endTime < finalEndTime ) {
        
        HourBin b = { startTime, 0 };
                    
        
        Commit *c = sortedList.getElement( index );
        
        while( index < sortedList.size() && 
               c->utcTime >= startTime && 
               c->utcTime < endTime ) {
            
            // commit in this time bin
            
            if( c->localTime.tm_hour == lastCounted.tm_hour &&
                c->localTime.tm_year == lastCounted.tm_year &&
                c->localTime.tm_yday == lastCounted.tm_yday ) {
                // already saw a commit this hour
                }
            else {
                // new hour
                
                lastCounted = c->localTime;
                

                if( c->considerFlag ) {
                    // and this commit is flagged as under consideration
                    b.numHoursIn ++;
                    }
                else {
                    // also track total of commits NOT considered
                    b.numHoursOut ++;
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
        HourBin *b = bins.getElement(i);
        
        time_t t = b->binStartTime;
        
        strftime( buff, 100, "%x", localtime( &t ) );

        int total = b->numHoursIn + b->numHoursOut;
        
        double fraction = 1.0;
        
        if( total > 0 ) {
            fraction = b->numHoursIn / (double)( total );
            }
        
        fprintf( inOutputFile, "%s %d %f\n", buff, b->numHoursIn, fraction );
        }
    }




void countWeeklyHoursOHOL() {
    for( int i=0; i<sortedList.size(); i++ ) {
        sortedList.getElement( i )->considerFlag = true;
        }
    
    const char *outName = "../hoursPerWeek_ohol.dat";
    
    FILE *outFile = fopen( outName, "w" );
    
    if( outFile == NULL ) {
        printf( "Failed to open output file %s\n", outName );
        return;
        }
    int weekSec = 3600 * 24 * 7;
    countBinnedHours( "Thu May 28 00:00:00 2015",
                      "Sun Jun 06 00:00:00 2019",
                      weekSec,
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
        

            // any single commits with more than 500 lines aren't handwritten
            // code (code imports, boilerplate, text databases, etc).
            if( ! found && c.linesAdded < 500 ) {
                sortedList.push_back( c );
                currentCommitBin.push_back( c );
                }
            else {
                delete [] c.rawLogLine;
                delete [] c.logName;
                }
            
            }
        
        countWeeklyLines();
        countWeeklyLinesOHOL();
        countMonthlyLines();
        countYearlyLines();
        countDayOfWeekLines( "Thu Jan 01 00:00:00 2004",
                             "Sun Jun 06 00:00:00 2019", 
                             "../locPerWeekday.dat" );
        countHourOfDayLines( "Thu Jan 01 00:00:00 2004",
                             "Sun Jun 06 00:00:00 2019", 
                             "../locPerHourOfDay.dat");

        countYearlyWednesdayLines();
        countYearlyFridayLines();
        
        countYearlyOnePMLines();
        countYearlyNineAMLines();

        countWeeklyHoursOHOL();

        
        countDayOfWeekLines( "Thu Jun 01 00:00:00 2017",
                             "Sun Sep 03 00:00:00 2017", 
                             "../locPerWeekday_oholBefore.dat" );
        countHourOfDayLines( "Thu Jun 01 00:00:00 2017",
                             "Sun Sep 03 00:00:00 2017", 
                             "../locPerHourOfDay_oholBefore.dat" );

        countDayOfWeekLines( "Sun Sep 03 00:00:00 2017",
                             "Sun Jun 06 00:00:00 2019", 
                             "../locPerWeekday_oholAfter.dat" );
        countHourOfDayLines( "Sun Sep 03 00:00:00 2017", 
                             "Sun Jun 06 00:00:00 2019", 
                             "../locPerHourOfDay_oholAfter.dat" );
        
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
