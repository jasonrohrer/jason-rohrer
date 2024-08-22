//test

$fn = 40;

//$fa = 40;
skull();



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
             
            linear_extrude(height = 20, center = false, convexity = 10, $fn=200 ) {
                import( "coinRimRidges.svg");
            }
            difference() {
                linear_extrude(height = 5, center = false, convexity = 10) {
                    import( "coinRim.svg");
                }
                translate( [0,0,2.5] ) {
                    linear_extrude(height = 5, center = false, convexity = 10) {
                        import( "coinRimText.svg");
                    }
                }
            }
            
            
            linear_extrude(height = 20, center = false, convexity = 10) {
                import( "skullRidge.svg");
            }
            difference() {
                linear_extrude(height = 5, center = false, convexity = 10) {
                    import( "skullOutline.svg");
                }
                linear_extrude(height = 100, center = true, convexity = 10) {
                    import( "skullHoles.svg");
                }
                linear_extrude(height = 100, center = true, convexity = 10) {
                    import( "skullLines.svg");
                }
                translate( [0,0,2.5] ) {
                    linear_extrude(height = 5, center = false, convexity = 10) {
                        import( "skullText.svg");
                    }
                }
            }
            
            
        }
    }
}