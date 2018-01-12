set output "dollarsPerGame.eps"

call 'graphSetup.gnuplot'

set boxwidth 0.5
set style fill solid border 0
set yrange [-40000:280000]
set xtics rotate by -45

#set xzeroaxis lt 1 lc rgb "#000000"
#unset yzeroaxis

set decimal locale
set format y "%'g"

set grid ytics linewidth 4
show grid

set lmargin 10


plot "dollarsPerGame.dat" using 1:3:xtic(2) with boxes fillcolor rgb "#666666"