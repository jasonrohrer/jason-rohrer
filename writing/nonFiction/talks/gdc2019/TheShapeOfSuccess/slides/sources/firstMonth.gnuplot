set output "firstMonth.eps"

call 'graphSetup.gnuplot'

set xrange [1:32]
set yrange [0:40000]

set xtics 1

set format x ''




#"<(sed -n '1000,2000p' filename.txt)"


plot "<(sed -n '1,3p' tcdFirstMonth.dat)" using 1:3 with linespoints pointtype 5linecolor 5 lt 1 lw 3, "<(sed -n '1,3p' oholFirstMonth.dat)"  using 1:3 with linespoints pointtype 7 linecolor 4 lt 1 lw 3
