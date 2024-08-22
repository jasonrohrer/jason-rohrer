//test


skull();



module skull() {
    scale( 1/250 ) {
        difference() {
            linear_extrude(height = 20, center = true, convexity = 10) {
                import( "skullOutline.svg");
            }
            linear_extrude(height = 100, center = true, convexity = 10) {
                import( "skullHoles.svg");
            }
            linear_extrude(height = 100, center = true, convexity = 10) {
                import( "skullLines.svg");
            }
        }
    }
}