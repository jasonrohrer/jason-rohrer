set output "graph1.png"

set yrange [0:21]

call 'graphSetup.gnuplot'


set xtics ('CoD:MW2' 1)

plot "graph1.data" using 1:2 with boxes
