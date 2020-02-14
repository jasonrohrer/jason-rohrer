for i in wholeCloth_*.txt; do
    

    parCount=`cat $i | wc -l`

    if [ $parCount -lt 4 ];
    then
	echo ""
	echo ""
	echo ""
	echo ""
	echo ""
	echo "====> $i"
	echo ""
	cat "$i"
    fi
done
