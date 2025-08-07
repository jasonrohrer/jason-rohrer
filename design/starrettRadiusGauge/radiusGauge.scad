
protrusionThickness = 0.0352;

endCoverHeight = 1/4;

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



module outerEdge() {
    import( file = "openings.dxf", layer="OuterEdge", $fn=30 );
}


module opening() {
    import( file = "openings.dxf", layer="OpeningBigger", $fn=30 );
}


module innerOpening() {
    import( file = "openings.dxf", layer="InnerOpeningBigger", $fn=30 );
}

