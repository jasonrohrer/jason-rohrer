

GRAPHS = \
dollarsPerGame.gnuplot \
dollarsPerWeek.gnuplot \
locPerGame.gnuplot \
locPerWeek.gnuplot \
weeksPerGame.gnuplot \
locPerWeek_time.gnuplot \
locPerMonth_time.gnuplot \
locPerYear_time.gnuplot \
locPerWeekday.gnuplot \
locPerHourOfDay.gnuplot \
locPerYearWednesdayFriday.gnuplot \
locPerYearNineOne.gnuplot \
locPerWeek_ohol.gnuplot \

GRAPHS_DAT = ${GRAPHS:.gnuplot=.dat}
GRAPHS_EPS = ${GRAPHS:.gnuplot=.eps}
GRAPHS_PNG = ${GRAPHS:.gnuplot=.png}


all:  ${GRAPHS_PNG}

%.eps: %.gnuplot %.dat graphSetup.gnuplot
	gnuplot $<

locPerYearWednesdayFriday.eps: locPerYearWednesdayFriday.gnuplot locPerYearWednesday.dat locPerYearFriday.dat graphSetup.gnuplot
	gnuplot locPerYearWednesdayFriday.gnuplot

locPerYearNineOne.eps: locPerYearNineOne.gnuplot locPerYearNineAM.dat locPerYearOnePM.dat graphSetup.gnuplot
	gnuplot locPerYearNineOne.gnuplot


%.png: %.eps
	convert -density 100 $< -flatten -negate -rotate 90 $@