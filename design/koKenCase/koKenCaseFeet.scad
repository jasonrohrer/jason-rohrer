
holeDepth=1/16 + 0.01;

//footWithMagnetHole();


/*
intersection() {
footWithMagnetHole();
    translate( [-0.25, -0.25, 0] )
    rotate( [0, 0, -45] )
    scale( [0.85, 1.1, 1] )
cylinder( h=5, d=1.2, center=true, $fn=200 );
}
*/
/*
intersection() {
footWithMagnetHole();
translate( [ 0.35, 0.35, 0 ] )
cylinder( h=5, d=.5, center=true, $fn=200 );
}
*/


intersection() {
footWithMagnetHole();
translate( [ 1.25, -0.5, 0 ] )
cylinder( h=5, d=1, center=true, $fn=200 );
}


module footWithMagnetHole() {
    difference() {
        cornerFoot();
        
        // bottom plate hole
        translate( [0,0, -holeDepth] )
            linear_extrude(height = 1/8 )
                magnetHole();
        
        // side wall hole
        translate( [1.25,-0.3125 - holeDepth, 0.2813] )
            rotate( [90,0,0] )
                linear_extrude(height = 1/8 )
                    magnetHole();
        // side wall hole
        translate( [-0.4375 - holeDepth, 0.75, 0.2813] )
            rotate( [90,0,90] )
                linear_extrude(height = 1/8 )
                    magnetHole();
    }
}  




module cornerFoot() {
    rotate_extrude(angle = 90, convexity = 2, $fn=200)
        crossSection();
    
    translate( [0,1.0,0] )
        rotate( [90,0,0] )
            linear_extrude(height = 1.0 )
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
    import( file = "koKenCaseFeet.dxf", layer="MagnetHole", $fn=60 );
}



module bottom() {
    import( file = "koKenCaseFeet.dxf", layer="Bottom", $fn=200 );
}
