set output "locPerYearNineOne.eps"

call 'graphSetup.gnuplot'

set xdata time
set timefmt "%m/%d/%Y"
set xrange ["01/01/2004":"1/1/2017"]
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


plot "locPerYearNineAM.dat" using 1:3 with lines lt 1 lw 5 linecolor rgb "#FF00FF", "locPerYearOnePM.dat" using 1:3 with lines lt 1 lw 5 linecolor rgb "#00FFFF"