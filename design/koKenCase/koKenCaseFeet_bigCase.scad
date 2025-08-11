
holeDepth=1/16 + 0.01;

holeExtrude=1/8;

cornerCrossSectionRadius = 0.59375;

bottomFoot = true;

footWithMagnetHole();

if( false )
intersection() {
footWithMagnetHole();
    translate( [.2,.2, 0] )
    rotate( [0, 0, 45] )
    scale( [1.8, 1.3, 1] )
cylinder( h=5, d=1.3, center=true, $fn=200 );
}

/*
intersection() {
footWithMagnetHole();
translate( [ 0.35, 0.35, 0 ] )
cylinder( h=5, d=.5, center=true, $fn=200 );
}
*/

/*
intersection() {
footWithMagnetHole();
translate( [ 1.25, -0.5, 0 ] )
cylinder( h=5, d=1, center=true, $fn=200 );
}
*/

module footWithMagnetHole() {
    difference() {
        cornerFoot();
        
        // bottom plate hole
        translate( [-.24,-.24, -holeDepth] )
            linear_extrude(height = holeExtrude )
                magnetHole();
        
        // side wall hole
        translate( [1.25,- (cornerCrossSectionRadius - holeExtrude) - holeDepth, 0.2813] )
            rotate( [90,0,0] )
                linear_extrude(height = holeExtrude )
                    magnetHole();
        // side wall hole
        translate( [ - cornerCrossSectionRadius - holeDepth, 0.75, 0.2813] )
            rotate( [90,0,90] )
                linear_extrude(height = holeExtrude )
                    magnetHole();
        
        if( bottomFoot ) {
            translate( [0,0, -5] )
            linear_extrude(height = 10 )
                    footHole();
        }
        
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
            if( bottomFoot ) 
                bottomFeet();
            else
                bottom();
}


module topLine() {
    import( file = "koKenCaseFeet_bigCase.dxf", layer="Top", $fn=30 );
}


module crossSection() {
    import( file = "koKenCaseFeet_bigCase.dxf", layer="CrossSectionForRot", $fn=200 );
}


module magnetHole() {
    import( file = "koKenCaseFeet_bigCase.dxf", layer="MagnetHole", $fn=60 );
}



module bottom() {
    import( file = "koKenCaseFeet_bigCase.dxf", layer="Bottom", $fn=200 );
}

module bottomFeet() {
    import( file = "koKenCaseFeet_bigCase.dxf", layer="BottomFeet", $fn=200 );
}


module footHole() {
    import( file = "koKenCaseFeet_bigCase.dxf", layer="footHole", $fn=200 );
}
