
footWithMagnetHole();



module footWithMagnetHole() {
    difference() {
        cornerFoot();
    
        translate( [0,0, -1/16 - 0.01] )
            linear_extrude(height = 1/8 )
                magnetHole();
    }
}  


module cornerFoot() {
    rotate_extrude(angle = 90, convexity = 2, $fn=200)
        crossSection();
    
    translate( [0,1.5,0] )
        rotate( [90,0,0] )
            linear_extrude(height = 1.5 )
                crossSection();

    translate( [0,0,0] )
        rotate( [90,0,90] )
            linear_extrude(height = 1.5 )
                crossSection();

    translate( [0,0,-1/8] )
        linear_extrude(height = 1/8 )
            bottom();
}


module topLine() {
    import( file = "koKenCaseFeet.dxf", layer="Top", $fn=30 );
}


module crossSection() {
    import( file = "koKenCaseFeet.dxf", layer="CrossSectionForRot", $fn=200 );
}


module magnetHole() {
    import( file = "koKenCaseFeet.dxf", layer="MagnetHole", $fn=30 );
}


module bottom() {
    import( file = "koKenCaseFeet.dxf", layer="Bottom", $fn=200 );
}
