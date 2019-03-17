

GRAPHS = \
firstMonthA.gnuplot \
firstMonthB.gnuplot \
firstMonthC.gnuplot \
firstMonthD.gnuplot \
firstMonthE.gnuplot \
firstMonthF.gnuplot \
firstMonthG.gnuplot \
firstMonthH.gnuplot \
firstMonthI.gnuplot \
secondMonthA.gnuplot \
secondMonthB.gnuplot \
secondMonthC.gnuplot \
secondMonthD.gnuplot \
secondMonthE.gnuplot \
secondMonthD2.gnuplot \
secondMonthE2.gnuplot \
witnessSubnauticaA.gnuplot \
witnessSubnauticaB.gnuplot \
witnessSubnauticaC.gnuplot \

GRAPHS_DAT = ${GRAPHS:.gnuplot=.dat}
GRAPHS_EPS = ${GRAPHS:.gnuplot=.eps}
GRAPHS_PNG = ${GRAPHS:.gnuplot=.png}


all:  ${GRAPHS_PNG}

%.eps: %.gnuplot graphSetup.gnuplot
	gnuplot $<

%.png: %.eps
	convert -density 100 $< -flatten -negate -rotate 90 $@