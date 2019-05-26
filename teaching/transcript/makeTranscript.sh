
inFile=mezTranscript.txt

name=`head -1 $inFile | tail -1 | awk '{print toupper($0)}'`
gender=`head -2 $inFile | tail -1 | awk '{print toupper($0)}'`
dob=`head -3 $inFile | tail -1 | awk '{print toupper($0)}'`

addrA=`head -4 $inFile | tail -1 | awk '{print toupper($0)}'`
addrB=`head -5 $inFile | tail -1 | awk '{print toupper($0)}'`

#cat transcriptBody.tex | sed -e "s/%NAME%/$name/" | sed -e "s/%GENDER%/$gender/" | sed -e "s/%DOB%/$dob/" | sed -e "s/%ADDR_A%/$addrA/" | sed -e "s/%ADDR_B%/$addrB/" > transcriptWorking.tex



newline=$'\n'

lineCount=0

curBlockCount=0
curBlock=""
while read p; do

	if [ "$lineCount" -gt "4" ]; then
		if [ "$p" != "" ]; then
			p=`echo -n "$p" | awk '{print toupper($0)}'`
			
			curBlock="$curBlock$p"
			curBlockCount=$((curBlockCount+1))
			if [ "$curBlockCount" -gt "4" ]; then
				curBlock="$curBlock\\\\$newline"
				curBlockCount=0
			else
				curBlock="$curBlock&"				
			fi
		fi
	fi
	lineCount=$((lineCount+1))
done <$inFile


# fixme:  can't stick curBlock in there like that, because of the slashes
cat transcriptBody.tex | sed -e "s/%NAME%/$name/" | sed -e "s/%GENDER%/$gender/" | sed -e "s/%DOB%/$dob/" | sed -e "s/%ADDR_A%/$addrA/" | sed -e "s/%ADDR_B%/$addrB/" > transcriptWorkingTemp.tex

grep -B 10000 "%COURSE_LINES%" transcriptWorkingTemp.tex > transcriptWorkingBefore.tex

grep -A 10000 "%COURSE_LINES%" transcriptWorkingTemp.tex > transcriptWorkingAfter.tex

echo -n "$curBlock" > transcriptWorkingMiddle.tex

cat transcriptWorkingBefore.tex transcriptWorkingMiddle.tex transcriptWorkingAfter.tex > transcriptWorking.tex


latex transcriptWorking.tex
dvips transcriptWorking.dvi
ps2pdf transcriptWorking.ps

evince transcriptWorking.pdf &

rm *Working*.tex *Working.log *Working.dvi *Working.aux *Working.ps