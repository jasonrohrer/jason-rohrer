$fn=100;

scale( 10 )

difference() {
    linear_extrude(height = 0.5, convexity = 1) {
        import(file = "address_caslon_5.dxf", layer="MOREVERTS" );
        import(file = "address_caslon_4.dxf", layer="MOREVERTS" );
        import(file = "address_caslon_A.dxf", layer="MOREVERTS" );
    }
    translate( [0,0,-0.01] ) {
        linear_extrude(height = 0.15, convexity = 1) {
            import(file = "address_caslon_5.dxf", layer="BIGGER_HOLES" );
            import(file = "address_caslon_4.dxf", layer="BIGGER_HOLES" );
            import(file = "address_caslon_A.dxf", layer="BIGGER_HOLES" );
        }
    }
}