$fn=100;


measuredDiameter = 2.379;

diameterTweak=0.03;

holeDiameter = measuredDiameter + diameterTweak;

crankHandleGap = 0.005;

// final
discHeight= 0.580;
extraDiameter=0.5;

// for test print
//discHeight= 0.1;
//extraDiameter = 0.2;


union() {
    mainRing();
    nubRing();
    crankArm();
    //testCrankEnd();
}



// for testing spin gap on knob
module testCrankEnd() {
    intersection() {
        cube( [1,1,1], center=true );
        armEnd();
    }
}


module mainRing() {
    difference() {
        cylinder( h=discHeight, d=holeDiameter + extraDiameter, center=true, $fn=100 );
        cylinder( h=discHeight+1, d=holeDiameter, center=true, $fn=100 );
    }
}

module nubRing() {
    for( i=[0:49] ) {
        rotate( a=[0,0, i * 360/50] ) {
            translate( [holeDiameter/2 +.02, 0, 0] ) {
                cylinder( h=discHeight, r=0.042 + .02, center=true, $fn=30 );
            }
        }
    }
}


module crankArm() {
    translate( [2.65,0,0])
        rotate( a=[0,90,0] )
            cube( [ discHeight, discHeight, 2.5], center=true );
    translate( [4.308,0,0])
        armEnd();
}


module armEnd() {
    difference() {
        cylinder( h=discHeight, d=1, center=true );
        translate( [0,0,-discHeight/2] )
        rotate_extrude($fn=200) {
            translate( [crankHandleGap + 0.001 ,0,0] ) {
                offset( delta=crankHandleGap ) {
                    crankHole2DShape();
                }
            }
        }
    }
    
    //if( false )
    translate( [0,0,- discHeight/2] )
    rotate_extrude($fn=200)
        crankHole2DShape();
    
    translate( [0,0,0.75 + discHeight/2 - 0.001 ] )
        cylinder( h=1.5, d=0.6, center=true );
    
    translate( [0,0, 1.5 + discHeight/2 - 0.001 ] )
    sphere( d=0.6 );
}


module crankHole2DShape() {
    polygon( points=[ [0,0], [0,discHeight], [0.3,discHeight], [0.2, discHeight/2], [0.3,0] ] );
}