call 'graphSetup.gnuplot'

set xrange [0:33]
set yrange [0:10000]

set xtics 1

set format x ''

#"<(sed -n '1000,2000p' filename.txt)"

set key on

set key font ",20"

set key spacing 3

set decimal locale
set format y "%'g"