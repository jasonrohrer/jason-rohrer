
echo "Generating for a series of $1 players"

bash makeTestDice

./testDice $1 > data.txt
./testDice2 $1 > data2.txt
./testDice3 $1 > data3.txt
./testDice4 $1 > data4.txt

gnuplot data.gnuplot
gnuplot dataBin.gnuplot
