set output "locPerWeek_time.eps"

call 'graphSetup.gnuplot'

set xdata time
set timefmt "%m/%d/%Y"
set xrange ["01/01/2004":"01/21/2018"]
#set xrange ["01/01/2016":"01/21/2018"]

set xtics "01/01/2004",31536000,"01/01/2018" out
#set xtics "01/01/2004",2628000,"01/01/2018" out	

set format x "%Y"
#set format x "%m/%Y"

set timefmt "%m/%d/%y"

#set xtics rotate by -45
#set xtic 52

set style fill solid noborder

set size 1, .8


plot "locPerWeek_time.dat" using 1:2 with filledcurves y1=0 fillcolor rgb "#000000"