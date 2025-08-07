
protrusionThickness = 0.0482;


difference() {
    linear_extrude(height = 1/4 )
        outerEdge();
    
    translate( [0, 0, -1 ] )
        linear_extrude(height = 1 + protrusionThickness + .01 )
            opening();
    
    translate( [0, 0, protrusionThickness ] )
        linear_extrude(height = 1/4 )
            innerOpening();
}



module outerEdge() {
    import( file = "openings.dxf", layer="OuterEdge", $fn=30 );
}


module opening() {
    import( file = "openings.dxf", layer="Opening", $fn=30 );
}


module innerOpening() {
    import( file = "openings.dxf", layer="InnerOpening", $fn=30 );
}

