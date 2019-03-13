call 'graphSetup.gnuplot'

set xrange [1:32]
set yrange [0:40000]

set xtics 1

set format x ''

#"<(sed -n '1000,2000p' filename.txt)"

set key on

set key font ",20"

set key spacing 3

set decimal locale
set format y "%'g"