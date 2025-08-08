
protrusionThickness = 0.0402;

endCoverHeight = 1/2;

legThickness = 0.3906;



union() {
difference() {
    linear_extrude(height = endCoverHeight )
        outerEdge();
    
    translate( [0, 0, -1 ] )
        linear_extrude(height = 1 + protrusionThickness + .01 )
            opening();
    
    translate( [0, 0, protrusionThickness ] )
        linear_extrude(height = endCoverHeight )
            innerOpening();
}




    color( "red")

    difference() {
        
        translate( [0, -1/8 , 0] )
            rotate( [-90,0,0] )
                linear_extrude(height = legThickness )
                    translate( [0, -0.2656, 0 ] )
                        legs();
        translate( [0, 0, -1 ] )
            linear_extrude(height = 2)
                opening();
    }
}
module outerEdge() {
    import( file = "openings.dxf", layer="OuterEdge", $fn=30 );
}


module opening() {
    import( file = "openings.dxf", layer="OpeningBigger", $fn=30 );
}


module innerOpening() {
    import( file = "openings.dxf", layer="InnerOpeningBigger", $fn=30 );
}


module legs() {
    import( file = "openings.dxf", layer="Legs", $fn=100 );
}
