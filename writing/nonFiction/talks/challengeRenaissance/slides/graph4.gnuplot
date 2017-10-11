set output "graph4.png"

set yrange [0:1050]

call 'graphSetup.gnuplot'


set xtics ('CoD:MW2' 1, 'Avatar' 3, 'Titanic' 5, "Gone With\nThe Wind" 7)

plot "graph4.data" using 1:2 with boxes
