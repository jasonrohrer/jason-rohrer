set output "locPerWeek.eps"

call 'graphSetup.gnuplot'

set boxwidth 0.5
set style fill solid border 0
set yrange [0:2400]
set xtics rotate by -45
plot "locPerWeek.dat" using 1:3:xtic(2) with boxes fillcolor rgb "#888888"