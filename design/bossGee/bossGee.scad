//test

$fn = 40;

//$fa = 40;
skull();

rotate( [0, 0, 42 ] )
sideSprue();

rotate( [0, 0, 140 ] )
sideSprue();

rotate( [0, 0, -19 ] )
sideSprue();


rotate( [0, 0, -80 ] )
sideSprue();


rotate( [0, 0, -118 ] )
sideSprue();


mainSprue();

module mainSprue() {
    translate( [0,0, .172 -.044 +.22489/2] ) {
        cylinder( h=0.66597, d=0.22489, center=false );
        sphere( d=0.22489 );
        }
    }

module sideSprue() {

    //translate( [0.336/2 - 0.07, 0, .172 + .02] )
    //sphere( r=0.044 );
    
    translate( [0, 0, .172] )
    rotate( [0, 90, 0 ] )
    translate( [0, 0, 0.658] )
    rotate( [0,180,0] )
    linear_extrude(height = .658, center = false, scale=[1.5,1.5])
        circle(r = 0.022);
        
    translate( [.658, 0, 0] )
    translate( [0, 0, 0.022] )
    rotate( [0,90,-90] )
    translate( [-0.075, 0, 0 ] )
    rotate_extrude(angle=180, convexity=10)
       translate([.075, 0]) circle(0.022);
    }
/*

The problem here is that we have the sprue connection points coming from SVG files,
which limits what we can do with them, because they're not centered.

So, for example, we can't make funnels at each point, or control the angle of the
curves very well.

Seems like we're going to have to actually pull the numerical coordinates out of the
spruePoint svg files, like this:

       cx="-462.13651"
       cy="745"
       r="6.0900416"
       
... so that we actually have something more flexible to work with.

Then we can construct the sprue in the center, and then translate it out to where
we need it to connect.

*/


module outerSprue() {
    translate( [0,0,19.9] ) {
        
        color( "red" )
        linear_extrude(height = 10, center = false, convexity = 10) {
            import( "spruePointOuter.svg");
        }
        
        translate( [0,0,9.9] )
        scale( [1,1,2] )
        rotate( [-90,0,0] )
        color( "green" )
        rotate_extrude(convexity = 10, angle=90 ) {
            import( "spruePointOuterLarger.svg");
        }
    }
}

module innerSprue() {
    translate( [0,0,19.9] ) {
        /*
        color( "red" )
        linear_extrude(height = 5, center = false, convexity = 10) {
            import( "spruePointOuter.svg");
        }
        */
        scale( [1,1,2.65] )
        rotate( [-90,0,0] )
        color( "green" )
        rotate_extrude(convexity = 10, angle=90 ) {
            import( "spruePointInner.svg");
        }
    }
}

module cheekSprue() {
    translate( [0,0,19.9] ) {
        /*
        color( "red" )
        linear_extrude(height = 5, center = false, convexity = 10) {
            import( "spruePointOuter.svg");
        }
        */
        scale( [1,1,4.3] )
        rotate( [-90,0,0] )
        color( "green" )
        rotate_extrude(convexity = 10, angle=90 ) {
            import( "spruePointCheek.svg");
        }
    }
}

module skull() {
    scale( 1/250 ) {
        union() {
        /*
            for( s=[0:5] ) {
                rotate( a=[0,0,60*s] ) {
                    outerSprue();
                    rotate( a=[0,0,30])
                        innerSprue();
                }
            }
            
            cheekSprue();
            rotate( [0,0,180] )
                cheekSprue();
          */
          
            linear_extrude(height =11, center = false, convexity = 10, $fn=150 ) {
                import( "coinRimRidges.svg");
            }
            difference() {
                linear_extrude(height = 11, center = false, convexity = 10) {
                    import( "coinRim.svg");
                }
                translate( [0,0,5.6] ) {
                    linear_extrude(height = 5.5, center = false, convexity = 10) {
                        import( "coinRimTextOverpass.svg");
                    }
                }
            }
            
            
            linear_extrude(height = 11, center = false, convexity = 10) {
                import( "skullRidge.svg");
            }
            difference() {
                linear_extrude(height = 11, center = false, convexity = 10) {
                    import( "skullOutline.svg");
                }
                linear_extrude(height = 100, center = true, convexity = 10) {
                    import( "skullHoles.svg");
                }
                linear_extrude(height = 100, center = true, convexity = 10) {
                    import( "skullLines.svg");
                }
                translate( [0,0,5.6] ) {
                    linear_extrude(height = 5.5, center = false, convexity = 10) {
                        import( "skullTextOverpass.svg");
                    }
                }
            }
            
            
        }
    }
}