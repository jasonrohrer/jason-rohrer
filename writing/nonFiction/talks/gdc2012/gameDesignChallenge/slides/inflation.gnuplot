set output "inflationGraph.eps"

#set yrange [-20:20]

set xrange [1915:2011]

call 'graphSetup.gnuplot'

set xlabel "Year"

set ylabel "Inflation %"

ymin=0


set title "Yearly Inflation Rate, USA"

plot "cpiAllData.txt" using 1:($16) with filledcurves y1=0 lt 2, "cpiAllData.txt" using 1:($16) with lines lt -1, 0 lt -1, 5 lt 3
