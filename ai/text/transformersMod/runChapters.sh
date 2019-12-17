echo "Running $2 chapters, starting with chapter $1, numerical seed $3 and input text file $4"


for (( c = 0; c < $2; c++ )) 
do
	chapter=$(($i + $1))

	seed=$3

	chapterDone="0"

	while [ $chapterDone -ne "1" ]
	do
		echo "Trying to generate Chapter $chapter with seed $seed"
		fileName=chapter${chapter}_${seed}.txt
		
		python ./run_generation.py --model_type=gpt2 --length=20 --model_name_or_path=gpt2-xl --out_file=$fileName --in_file=$4 --gen_words=4000 --seed=$seed --stop_token="<|endoftext|>"
		
		if grep -q "END OF CHAPTER" $fileName; then
			chapterDone="1"
		fi
		seed=$((1 + $seed))
	done
done