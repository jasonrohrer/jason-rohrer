# pipe in chapter text

# pipe out processed text


stripSubs=0
if [[ $1 == "-stripSubHeadings" ]]; then
	stripSubs=1
fi


IFS=

lineNum=0

isEpilogue=0
while read -r line; 
do 
    #echo -n "$lineNum"

	if [[ $line == "Epilogue" ]]; then
		isEpilogue=1
	fi

	if [ $lineNum -eq 6 ] && [ $isEpilogue -ne 1 ]; then
		size=${#line}
		
		if [ $size -lt "60" ]; then
			if [[ ! $line == *"."* ]] && [[ ! $line == *"\""* ]] && [[ ! $line == *":"* ]]; then
				if [ $stripSubs -eq 1 ]; then
					# strip it out and skip it
					# no-op here to keep if-else happy
					blah=1
				else
					lineUpper=${line^^}
					echo $lineUpper
					echo ""
					echo ""
				fi
			else
				echo $line
			fi
		else
			echo $line
		fi
	else
		echo $line
	fi
	
	lineNum=$(($lineNum + 1))
done
