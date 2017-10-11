set output "graph3.png"

set yrange [0:420]

call 'graphSetup.gnuplot'


set xtics ('CoD:MW2' 1, 'Avatar' 3, 'Titanic' 5)

plot "graph3.data" using 1:2 with boxes
