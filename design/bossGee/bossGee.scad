//test

//$fn = 500;

//$fa = 40;
skull();



module skull() {
    scale( 1/250 ) {
        union() {
            
            linear_extrude(height = 10, center = false, convexity = 10, $fn=200 ) {
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
            
            
            linear_extrude(height = 10, center = false, convexity = 10) {
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