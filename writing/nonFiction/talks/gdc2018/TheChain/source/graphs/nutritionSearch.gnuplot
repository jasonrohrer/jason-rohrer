set output "nutritionSearch.eps"

call 'graphSetup.gnuplot'

set xdata time
set timefmt "%m/%d/%Y"
set xrange ["01/01/2013":"01/21/2018"]
#set xrange ["01/01/2016":"01/21/2018"]

set xtics "01/01/2013",31536000,"01/01/2018" out
#set xtics "01/01/2004",2628000,"01/01/2018" out	

set format x "%Y"
#set format x "%m/%Y"

set timefmt "%Y-%m-%d"

#set xtics rotate by -45
#set xtic 52

#set decimal locale
#set format y "%'g"

set yrange [0:110]

set style fill solid noborder

set size 1, .8


plot "nutritionSearch.dat" using 1:2 with lines lw 4 lc "#000000"