set output "dollarsPerWeek.eps"

call 'graphSetup.gnuplot'

set boxwidth 0.5
set style fill solid border 0
set yrange [-714.2:5000]

set decimal locale
set format y "$%'g"


#set xzeroaxis lt 1 lc rgb "#000000"
#unset yzeroaxis


 
plot "dollarsPerWeek.dat" using 1:3:xtic(2) with boxes fillcolor rgb "#666666"