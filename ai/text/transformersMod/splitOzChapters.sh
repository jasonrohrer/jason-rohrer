
rm -rf ozChapters

mkdir ozChapters

chapNum="1"

outFile="ozChapters/$chapNum.txt"

while read l; do
	if [[ "$l" =~ ^[0-9]+. ]]; then
		chapNum=$(($chapNum + 1))
		outFile="ozChapters/$chapNum.txt"
	else
		echo "$l" >> $outFile
	fi
done <oz.txt