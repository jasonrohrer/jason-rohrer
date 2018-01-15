set output "locPerHourOfDay_oholAfter.eps"

call 'graphSetup.gnuplot'

set boxwidth 0.5
set style fill solid border 0
set yrange [0:0.2]
set xtics rotate by -45

set grid ytics linewidth 4
show grid

set lmargin 10


plot "locPerHourOfDay_oholAfter.dat" using 1:4:xtic(2) with boxes fillcolor rgb "#666666"