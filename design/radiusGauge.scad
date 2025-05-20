thick=0.25;
letterThick = thick/8;
extra =0.01;

difference() {
linear_extrude(height = thick, center = true, convexity = 10)
    outline();
translate( [0,0,thick/2 - letterThick/2 + extra/2] )
    linear_extrude(height = letterThick + extra, center = true, convexity = 10)
        Text(); 
}


translate( [0, 0, thick/2 - letterThick/2] )
    color( "red" )
        linear_extrude(height = letterThick, center = true, convexity = 10)
            Text(); 

module outline() {
    import( file = "radiusGauge.dxf", layer="0", $fn=200 );
}


module Text() {
    import( file = "radiusGauge.dxf", layer="TextLines", $fn=20 );
}