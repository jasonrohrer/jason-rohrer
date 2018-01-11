

GRAPHS = \
dollarsPerGame.gnuplot \
dollarsPerWeek.gnuplot \
locPerGame.gnuplot \
locPerWeek.gnuplot \
weeksPerGame.gnuplot \

GRAPHS_DAT = ${GRAPHS:.gnuplot=.dat}
GRAPHS_EPS = ${GRAPHS:.gnuplot=.eps}
GRAPHS_PNG = ${GRAPHS:.gnuplot=.png}


all:  ${GRAPHS_PNG}

%.eps: %.gnuplot %.dat
	gnuplot $<

%.png: %.eps
	convert -density 100 $< -flatten -negate -rotate 90 $@