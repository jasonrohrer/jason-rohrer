set output "cpiGraph.eps"

set yrange [-20:250]

#set xrange [1914:2011]
set xrange [1913.125:2012.0416666666666667]

call 'graphSetup.gnuplot'

set xlabel "Year"

set ylabel "Consumer Price Index"



set title "Consumer Price Index, USA"

#plot "cpiAllData.txt" using 1:($14) with filledcurves y1=0 lt 2, "cpiAllData.txt" using 1:($14) with lines lt -1, 0 lt -1, 1.0575**(x-1915) + 10 lt 3

plot "cpiMonthly.txt" using 1:2 with filledcurves y1=0 lt 2, "cpiMonthly.txt" using 1:2 with lines lt -1, 0 lt -1, 1.0575**(x-1915) + 10 lt 3
