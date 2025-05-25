
plateThick = 1/4;

tallOffset = 3/4;


/*
plateThick = 1/8;

tallOffset = 1/2;
*/

riseOffset = 0.188;

holder();

/*
intersection() {
holder();

translate( [3.1,2,0] )
cylinder( h=5, d=6, center=true, $fn=200 );
}
*/

module holder() {
    linear_extrude(height = plateThick )
                    thinParts();

    linear_extrude(height = tallOffset + plateThick )
                    thickParts();

    linear_extrude(height = plateThick + .188 )
                    riseParts();
}


module thinParts() {
    import( file = "kestrelFourInchHolder.dxf", layer="Mount", $fn=200 );
}


module thickParts() {
    import( file = "kestrelFourInchHolder.dxf", layer="MountThick", $fn=200 );
}


module riseParts() {
    import( file = "kestrelFourInchHolder.dxf", layer="MountRise", $fn=200 );
}