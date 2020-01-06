reqWord=`cat $6`

echo "Running $2 chapters, starting with chapter $1, numerical seed $3 and input text file $4, and generating $5 versions of each chapter, requiring the word from file $6, '$reqWord', and extra argument $7"


minWords=2000
maxWords=4000


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
			python ./run_generation.py --model_type=gpt2 --length=20 --model_name_or_path=gpt2-xl --out_file=$overrunFileName --in_file=$inputFile --chapter_number=$chapter --gen_words=$maxWords --gen_min_words=$minWords --seed=$seed --stop_token="<|endoftext|>" $7
			
			if grep -q "END OF CHAPTER" $overrunFileName; then
				
				words=`wc -w $overrunFileName | cut -f1 -d' '`

				if ! grep -q "$reqWord" $overrunFileName; then
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