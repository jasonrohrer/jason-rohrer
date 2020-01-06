# pipe in chapter text

# pipe out processed text

lineNum=0
while read -r line; 
do 
	#echo -n "$lineNum"

	if [ $lineNum -eq 7 ]; then
		size=${#line}
		
		if [ $size -lt "60" ]; then
			lineUpper=${line^^}
			echo $lineUpper
			echo ""
			echo ""
		else
			echo $line
		fi
	else
		echo $line
	fi
	
	lineNum=$(($lineNum + 1))
done
