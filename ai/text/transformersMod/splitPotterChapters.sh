
rm -rf potterChapters

mkdir potterChapters

chapNum="1"

outFile="potterChapters/$chapNum.txt"

linesOut=0

while read l; do
	if [[ "$l" =~ ^[0-9]+.\ [A-Z\ ]+[A-Z\ ]+ ]]; then
		chapNum=$(($chapNum + 1))
		outFile="potterChapters/$chapNum.txt"
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
done <harryPotter.txt