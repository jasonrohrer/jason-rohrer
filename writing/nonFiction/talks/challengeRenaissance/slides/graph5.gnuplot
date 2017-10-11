set output "graph5.png"

set yrange [0:2100]

call 'graphSetup.gnuplot'


set xtics ('CoD:MW2' 1, 'Avatar' 3, 'Titanic' 5, "Gone With\nThe Wind" 7, "World Pop.\n1939" 9)

plot "graph5.data" using 1:2 with boxes
