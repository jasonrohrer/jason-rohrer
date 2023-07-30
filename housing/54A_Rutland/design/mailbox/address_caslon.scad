$fn=200;

scale( 10 )

// FIXME:  need to use latest version to
// be able to control faceting on import

difference() {
    linear_extrude(height = 0.5, convexity = 1) {
        import(file = "address_caslon_5.dxf", layer="Vector_Middle" );
        import(file = "address_caslon_4.dxf", layer="Vector_Middle" );
        import(file = "address_caslon_A.dxf", layer="Vector_Middle" );
    }
    translate( [0,0,-0.01] ) {
        linear_extrude(height = 0.15, convexity = 1) {
            import(file = "address_caslon_5.dxf", layer="Holes" );
            import(file = "address_caslon_4.dxf", layer="Holes" );
            import(file = "address_caslon_A.dxf", layer="Holes" );
        }
    }
}