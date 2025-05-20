letterThick=1/64.0;



/*
linear_extrude(height = 1/32, center = false, convexity = 10)
    outline( "32" );

linear_extrude(height = 1/16, center = false, convexity = 10)
    outline( "16" );

linear_extrude(height = 1/16, center = false, convexity = 10)
    outline( "16" );
*/

module allShims() {
    shim( 32 );
    shim( 16 );
    shim( 8 );
    shim( 4 );
    shim( 2 );
    shim( 1 );
}

difference() {
    allShims();
    translate( [0,0,1 - letterThick] )
        linear_extrude(height = 2 * letterThick, center = false, convexity = 10)
            text();
}


color( "red" )
translate( [0,0,1 - letterThick] )
linear_extrude(height = letterThick, center = false, convexity = 10)
    text();

linear_extrude(height = 1, center = false, convexity = 10)
    refPosts();



module shim( inThicknessDenom ) {
    translate( [0,0, 1 - 1/inThicknessDenom] )
        linear_extrude(height = 1/inThicknessDenom, center = false, convexity = 10)
            outline( inThicknessDenom );
}

module text() {
    import( file = "drillDepthSetupBlocks.dxf", layer="TextLines", $fn=30 );
}

module refPosts() {
    import( file = "drillDepthSetupBlocks.dxf", layer="RefPosts", $fn=200 );
} 

module outline( inLayerName ) {
    import( file = "drillDepthSetupBlocks.dxf", layer=inLayerName, $fn=200 );
}