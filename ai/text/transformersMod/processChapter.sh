# pipe in chapter text

# pipe out processed text

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
	else
		echo $line
	fi
	
	lineNum=$(($lineNum + 1))
done
