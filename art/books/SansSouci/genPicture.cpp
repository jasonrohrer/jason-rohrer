
#include "minorGems/graphics/Image.h"
#include "minorGems/graphics/converters/TGAImageConverter.h"
#include "minorGems/io/file/FileOutputStream.h"

#include "minorGems/util/stringUtils.h"


#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <time.h>


#include "random.h"



static void usage() {
    printf( "Usage:\n" );

    printf( "  genPicture width height\n" );
    
    printf( "Example:\n" );

    printf( "  genPicture 640 490\n" );
    

    exit( 0 );
    }



int w, h;


Image *im;

Color black( 0, 0, 0, 1 );
Color white( 1, 1, 1, 1 );



// sets pixel relative to image center
static void setPixelRel( int inX, int inY, int inValue ) {
    
    int x = inX + w / 2;
    int y = inY + h / 2;
    
    int index = y * w + x;
    
    if( inValue == 0 ) {
        im->setColor( index, black );
        }
    else {
        im->setColor( index, white );
        }
    }


// set pixel absolute
static void setPixel( int inX, int inY, int inValue ) {
    
    int index = inY * w + inX;
    
    if( inValue == 0 ) {
        im->setColor( index, black );
        }
    else {
        im->setColor( index, white );
        }
    }


// set pixel absolute
static void setPixel( int inX, int inY, Color inC ) {
    
    int index = inY * w + inX;
    im->setColor( index, inC );
    }





// set pixel absolute
static void addToPixel( int inX, int inY ) {
    
    int index = inY * w + inX;
    
    Color c = im->getColor( index );
    
    float increment = 0.5;

    c.r += increment;
    if( c.r > 1 ) {
        c.r = 1;
        }
    c.g += increment;
    if( c.g > 1 ) {
        c.g = 1;
        }
    c.b += increment;
    if( c.b > 1 ) {
        c.b = 1;
        }
    
    im->setColor( index, c );
    }


static int getPixel( int inX, int inY ) {
    int index = inY * w + inX;
    
    Color c = im->getColor( index );
    
    if( c.r == 0 && c.g == 0 && c.b == 0 ) {
        return 0;
        }
    else {
        return 1;
        }
    }


static Color getPixelColor( int inX, int inY ) {
    int index = inY * w + inX;
    
    return im->getColor( index );
    }



void test( int inRadius ) {
    
    for( int y=-inRadius; y<inRadius; y++ ) {
        for( int x=-inRadius; x<inRadius; x++ ) {
            
            if( sqrt( x * x + y * y ) > 20 * sin( x/10.0 )  + inRadius / 2 ) {
                setPixelRel( x, y, 0 );
                }
            else {
                setPixelRel( x, y, 1 );
                }
            }
        }

    }



void mandel( int inRadius ) {
    
    for( int y=-inRadius; y<inRadius; y++ ) {
        for( int x=-inRadius; x<inRadius; x++ ) {
           
            double x0 = 1.5 * x / (double) inRadius;

            double y0 = 1.5 * y / (double) inRadius;
            

            double xN = x0;
            double yN = y0;
            
            int numSteps = 0;
            
            int maxSteps = 1000;

            while( xN > -2 && xN < 1 && yN > -2 && yN < 1 && 
                   numSteps < maxSteps ) {
            
                double xNew = xN * xN - yN * yN + x0;

                double yNew = 2 * xN * yN + y0;
                
                xN = xNew;
                yN = yNew;
                numSteps ++;
                }
            
            if( numSteps < maxSteps ) {
                setPixelRel( x, y, 1 );
                }
            else {
                setPixelRel( x, y, 0 );
                }
            }
        }

    }

static float getRandomFloat() {
    return getRandom( RAND_MAX ) / (float)RAND_MAX;
    }



static int coinFlip() {
    float value = getRandom( RAND_MAX ) / (float)RAND_MAX;
    
    if( value > 0.5 ) {
        return 1;
        }
    else {
        return 0;
        }
    }


static int randomMove() {
    float value = getRandom( RAND_MAX ) / (float)RAND_MAX;
    
    if( value >= 0.5 ) {
        return 1;
        }
    else {
        return -1;
        }
    }



static Color getRandomColor() {
    Color *c = Color::makeColorFromHSV( getRandomFloat(), 1, 
                                        getRandomFloat() );
    
    return *c;
    }






void test2() {
    setPixel( w/2, 0, 1 );
    
    for( int y=1; y<h-1; y++ ) {
        for( int x=1; x<w-1; x++ ) {
            
            if( getPixel( x - 1, y - 1 ) !=
                getPixel( x + 1, y - 1 ) ) {
                
                setPixel( x, y, 1 );
                
                }
            }
        }
    
    }



void test3() {
    
    int x = w/2;
    int y = h/2;
    
    while( x > 0 && x < w - 1
           &&
           y > 0 && y < h - 1 ) {

        addToPixel( x, y );

        if( coinFlip() ) {
            x += randomMove();
            }
        if( coinFlip() ) {
            y += randomMove();
            }
        
        }
    }




void test4() {
    
    int x = w/2;
    int y = h/2;
    
    for( int j=0; j<1000; j++ ) {
        
        int xDest = x;
        int yDest = y;

        if( x % 29 == 0 && y % 29 != 0) {
            
            int m = randomMove();
            xDest += m * ( (y * 37 ) % (x + 31) );
            
            xDest = (xDest+2*w) % w;

            int d = 1;
            
            if( xDest < x ) {
                d = -1;
                }

            for( int i=x; i!=xDest; i += d ) {
                addToPixel( i, y );
                }
            x = xDest;
            }
        else {
            
            int m = randomMove();
            yDest += m * ( ( x*37) % (y+31) );
            
            yDest = (yDest+ 2*h) % h;

            int d = 1;
            
            if( yDest < y ) {
                d = -1;
                }

            for( int i=y; i!=yDest; i += d ) {
                addToPixel( x, i );
                }
            y = yDest;
            }
        }
    }


static Color randomColorStep( Color inC ) {

    Color other = getRandomColor();
    
    Color *step = Color::linearSum( &inC, &other, 0.95 );

    Color returnColor = *step;

    delete step;

    return returnColor;
    }





void test5() {
    
    for( int y=0; y<h; y++ ) {
        for( int x=0; x<w; x++ ) {
            
            setPixel( x, y, getRandomColor() );
            }
        }

    for( int i=0; i<1; i++ ) {
        for( int y=0; y<h; y++ ) {
            for( int x=0; x<w; x++ ) {
                
                Color sumColor( 0, 0, 0 );
                
                for( int dy = -1; dy <= 1; dy++ ) {
                    for( int dx = -1; dx <= 1; dx++ ) {
                        
                        int ny = y + dy;
                        int nx = x + dx;
                        
                        if( ny >= h ) {
                            ny -= h;
                            }
                        else if( ny < 0 ) {
                            ny += h;
                            }
                        
                        if( nx >= w ) {
                            nx -= w;
                            }
                        else if( nx < 0 ) {
                            nx += w;
                            }
                        


                        Color neighborColor = getPixelColor( nx, ny );

                        for( int c=0; c<3; c++ ) {
                            sumColor[c] += neighborColor[c];
                            }
                        }
                    }
                for( int c=0; c<3; c++ ) {
                    sumColor[c] /= 9;
                    }
                

                setPixel( x, y, sumColor );
                }
            }
        }
    }






void test6() {
    
    for( int y=0; y<h; y++ ) {
        Color startColor = getRandomColor();
        Color endColor = getRandomColor();
        
        for( int x=0; x<w; x++ ) {
            
            Color *blend;

            if( x < w/2 ) {
                blend = Color::linearSum( &endColor, &startColor,
                                          x / (float) (w/2) );
                }
            else {
                blend = Color::linearSum( &startColor, &endColor,
                                          ( x - w/2 ) / (float) (w/2) );
                }

            setPixel( x, y, *blend );
            
            delete [] blend;
            }
        }

    Color tempColors[h][w];

    for( int y=0; y<h; y++ ) {
        for( int x=0; x<w; x++ ) {
                
            Color sumColor( 0, 0, 0 );
            
            for( int dy = -1; dy <= 1; dy++ ) {
                for( int dx = -1; dx <= 1; dx++ ) {
                        
                    int ny = y + dy;
                    int nx = x + dx;
                    
                    if( ny >= h ) {
                        ny -= h;
                        }
                    else if( ny < 0 ) {
                        ny += h;
                        }
                    
                    if( nx >= w ) {
                        nx -= w;
                        }
                    else if( nx < 0 ) {
                        nx += w;
                        }
                        
                    
                    
                    Color neighborColor = getPixelColor( nx, ny );

                    for( int c=0; c<3; c++ ) {
                        sumColor[c] += neighborColor[c];
                        }
                    }
                }
            for( int c=0; c<3; c++ ) {
                sumColor[c] /= 9;
                }
            
            
            tempColors[y][x] = sumColor;
            }
        }
    
    for( int y=0; y<h; y++ ) {
        for( int x=0; x<w; x++ ) {
            setPixel( x, y, tempColors[y][x] );
            }
        }       

    char markedPixels[h][w];
    
    for( int y=0; y<h; y++ ) {
        for( int x=0; x<w; x++ ) {
            markedPixels[y][x] = false;
            }
        }
    
    for( int j=0; j<3; j++ ) {
        
        int x = w/2;
        int y = h/2;
        
        int hitEdge = false;
        for( int s=0; s<2 *w && !hitEdge; s++ ) {
        
            markedPixels[y][x] = true;
            markedPixels[y][w-x-1] = true;

            if( coinFlip() ) {
                x += randomMove();
                }
            else {
                y += randomMove();
                }
            
            if( x < 0 || x >= w || y < 0 || y >= h ) {
                hitEdge = true;
                }
            }

        }
    
    for( int y=0; y<h; y++ ) {
        for( int x=0; x<w; x++ ) {
    
            if( ! markedPixels[y][x] ) {
                setPixel( x, y, white );
                }
            }
        }
    }






int main( int inNumArgs, const char **inArgs ) {

    //srand( time( NULL ) );
    setRandomSeed( time( NULL ), time( NULL ) + 22573 );
    

    if( inNumArgs != 3 ) {
        usage();
        }

    
    int numRead = sscanf( inArgs[1], "%d", &w );
    
    if( numRead != 1 ) {
        usage();
        }

    numRead = sscanf( inArgs[2], "%d", &h );
    
    if( numRead != 1 ) {
        usage();
        }
    

    int r;
    
    if( w > h ) {
        r = h / 2 - 1;
        }
    else {
        r = w / 2 - 1;
        }
    

    for( int i = 0; i<100; i++ ) {
        im = new Image( w, h, 3 );



        //mandel( r );
        test6();

        char *fileName = autoSprintf( "out%03d.tga", i );

        File f( NULL, fileName );
    
        delete [] fileName;

        FileOutputStream fOut( &f );
        
        TGAImageConverter tga;
    
        tga.formatImage( im, &fOut );
    
        delete im;
        }
    
    

    return 0;
    }
