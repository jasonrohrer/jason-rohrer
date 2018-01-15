

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
hoursPerWeek_ohol.gnuplot \
locPerWeekday_oholBefore.gnuplot \
locPerHourOfDay_oholBefore.gnuplot \
locPerWeekday_oholAfter.gnuplot \
locPerHourOfDay_oholAfter.gnuplot \
locPerWeekWednesdayFriday_ohol.gnuplot \
locPerWeekNineOne_ohol.gnuplot \


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



locPerWeekWednesdayFriday_ohol.eps: locPerWeekWednesdayFriday_ohol.gnuplot locPerWeekWednesday_ohol.dat locPerWeekFriday_ohol.dat graphSetup.gnuplot
	gnuplot locPerWeekWednesdayFriday_ohol.gnuplot

locPerWeekNineOne_ohol.eps: locPerWeekNineOne_ohol.gnuplot locPerWeekNineAM_ohol.dat locPerWeekOnePM_ohol.dat graphSetup.gnuplot
	gnuplot locPerWeekNineOne_ohol.gnuplot




%.png: %.eps
	convert -density 100 $< -flatten -negate -rotate 90 $@