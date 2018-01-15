set output "locPerWeekNineOne_ohol.eps"

call 'graphSetup.gnuplot'

set xdata time
set timefmt "%m/%d/%Y"
set xrange ["06/01/2017":"1/1/2018"]
#set xrange ["01/01/2016":"01/21/2018"]

set xtics "06/01/2017",31536000,"01/01/2018" out
#set xtics "01/01/2004",2628000,"01/01/2018" out	

set format x "%Y"
#set format x "%m/%Y"

set timefmt "%m/%d/%y"

#set xtics rotate by -45
#set xtic 52

set style fill solid noborder

set size 1, .8


plot "locPerWeekNineAM_ohol.dat" using 1:3 with lines lt 1 lw 5 linecolor rgb "#FF00FF", "locPerWeekOnePM_ohol.dat" using 1:3 with lines lt 1 lw 5 linecolor rgb "#00FFFF"