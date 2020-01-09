reqWords=`cat $6`


IFS=',' read -r -a reqWordList <<< "$reqWords"

numReqWords=${#reqWordList[@]}

echo "Running $2 chapters, starting with chapter $1, numerical seed $3 and input text file $4, and generating $5 versions of each chapter, requiring the $numReqWords words from file $6, '$reqWords', $7 min words per chapter, $8 max words per chapter, and extra argument $9"


minWords=$7
maxWords=$8


for (( c = 0; c < $2; c++ )) 
do
	chapter=$(($c + $1))

	seed=$3

	for (( t = 0; t < $5; t++ )) 
	do
		chapterDone="0"
		
		while [ $chapterDone -ne "1" ]
		do
			echo "Trying to generate Chapter $chapter (try $t) with seed $seed"
			fileName=chapter${chapter}.${t}_${seed}_complete.txt

			offTopicFileName=chapter${chapter}.${t}_${seed}_offTopic.txt

			tooShortFileName=chapter${chapter}.${t}_${seed}_tooShort.txt
			
			overrunFileName=chapter${chapter}.${t}_${seed}_overrun.txt
			
			rm $fileName
			rm $overrunFileName
			
			inputFile=$4
			
			

			if [[ -d $inputFile ]]; then
				inputFileWorking=$inputFile
				
				echo "Input file $inputFile is a directory..."
				# a whole directory full of seed files
				fileIndex=1
				for f in `ls -1v $inputFile` 
				do
					if [ $fileIndex -eq $chapter ]; then
						inputFileWorking="$inputFile/$f"
					fi
					fileIndex=$(($fileIndex + 1))
				done
				inputFile=$inputFileWorking
				echo "Settling on input file $inputFile for chapter $chapter"
			fi
			
			# unbuffered std out (even if we're redirected to a file externally)
			python ./run_generation.py --model_type=gpt2 --length=20 --model_name_or_path=gpt2-xl --out_file=$overrunFileName --in_file=$inputFile --chapter_number=$chapter --gen_words=$maxWords --gen_min_words=$minWords --seed=$seed --stop_token="<|endoftext|>" $9
			
			if grep -q "END OF CHAPTER" $overrunFileName; then
				
				words=`wc -w $overrunFileName | cut -f1 -d' '`

				offTopic=0

				for word in "${reqWordList[@]}"
				do
					# count number of times required word occurs w/in
					# last 500 words of seed file
					countInSourceFile=`cat $inputFile | tr ' ' '\n' | tail -500 | grep -c Atticus`
					if [ $countInSourceFile -gt 3 ]; then
						echo "Seed file tail has $countInSourceFile occurrences of required '$word'"
						echo "checking output file for required word '$word'"
						if ! grep -q "$word" $overrunFileName; then
							echo "'$word' not found in output file $overrunFileName"
							offTopic=1
						fi
					else
						echo "Seed file tail has only $countInSourceFile occurrences of required '$word', skipping"
					fi
				done

				if [ $offTopic -eq 1 ]; then
					mv $overrunFileName $offTopicFileName
				elif [ "$words" -lt "$minWords" ]; then
					mv $overrunFileName $tooShortFileName
				else
					chapterDone="1"
					mv $overrunFileName $fileName
				fi
			fi
			
			seed=$((1 + $seed))
		done
	done
done