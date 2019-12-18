echo "Running $2 chapters, starting with chapter $1, numerical seed $3 and input text file $4, and generating $5 versions of each chapter"



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
			fileName=chapter${chapter}.${t}_${seed}.txt
			
			overrunFileName=chapter${chapter}.${t}_${seed}_overrun.txt
			
			rm $fileName
			rm $overrunFileName

			python ./run_generation.py --model_type=gpt2 --length=20 --model_name_or_path=gpt2-xl --out_file=$overrunFileName --in_file=$4 --chapter_number=$chapter --gen_words=4000 --seed=$seed --stop_token="<|endoftext|>"
			
			if grep -q "END OF CHAPTER" $overrunFileName; then
				chapterDone="1"
				mv $overrunFileName $fileName
			fi
			
			seed=$((1 + $seed))
		done
	done
done