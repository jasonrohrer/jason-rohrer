set output "graph2.png"

set yrange [0:365]

call 'graphSetup.gnuplot'


set xtics ('CoD:MW2' 1, 'Avatar' 3)

plot "graph2.data" using 1:2 with boxes
