set output "locPerGame.eps"

call 'graphSetup.gnuplot'

set boxwidth 0.5
set style fill solid border 0
set yrange [0:100000]
set xtics rotate by -45

#set xzeroaxis lt 1 lc rgb "#000000"
#unset yzeroaxis

set decimal locale
set format y "%'g"

set grid ytics linewidth 4
show grid

set lmargin 10


plot "locPerGame.dat" using 1:3:xtic(2) with boxes fillcolor rgb "#888888"