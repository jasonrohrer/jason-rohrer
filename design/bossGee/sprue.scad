$fn=50;

union() {
    // connects to benchy
    //color( [1, 0, 1, 0.5] )
    //cylinder( h=0.911, d=.140, center=true );
    
    // goes into rubber cylinder lid
    color( [1, 0, 1, 0.5] )
    translate( [0,0, 0.995/2 + 0.911/2 + 0.25] )
        cylinder( h=0.995, d=0.336, center=true );

    /*
    color( [1, 1, 0, 0.5] )
    // connects the two
    translate( [0,0, 0.911/2 + 0.25/2] )
        cylinder( h=0.26, d1=.140, d2=0.336, center=true );
    */
    
    // wider connector
    color( [1, 1, 0, 0.5] )
    // connects the two
    translate( [0,0, 0.911/2 + 0.25/2] )
        cylinder( h=0.26, d1=.2, d2=0.336, center=true );
    
}