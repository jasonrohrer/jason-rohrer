
rm -rf ozChapters

mkdir ozChapters

chapNum="1"

outFile="ozChapters/$chapNum.txt"

linesOut=0

while read l; do
	if [[ "$l" =~ ^[0-9]+. ]]; then
		chapNum=$(($chapNum + 1))
		outFile="ozChapters/$chapNum.txt"
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
done <oz.txt