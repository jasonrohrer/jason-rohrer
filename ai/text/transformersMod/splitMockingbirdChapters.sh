
rm -rf mockingbirdChapters

mkdir mockingbirdChapters

chapNum="1"

outFile="mockingbirdChapters/$chapNum.txt"

linesOut=0

while read l; do
	if [[ "$l" =~ ^[0-9]+ ]]; then
		chapNum=$(($chapNum + 1))
		outFile="mockingbirdChapters/$chapNum.txt"
	else
		if [[ ! -z "$l" ]]; then
			if [[ $linesOut -ne 0 ]]; then
				echo "" >> $outFile
				echo "" >> $outFile
			fi	
			echo -n "$l" >> $outFile
			linesOut=1
		fi
	fi
done <mockingbird.txt