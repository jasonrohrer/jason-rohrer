

echo "Running $2 seeded runs, starting with seed $1"

for (( i = 0; i < $2; i++ )) 
do
	seed=$(($i + $1))
	echo "seed $seed"

	python ./run_generation.py --model_type=gpt2 --length=20 --model_name_or_path=gpt2-xl --out_file=out${seed}_10000_noEnd_reseed.txt --in_file=sampleInputShort.txt --gen_words=10000 --seed=$seed --stop_token="<|endoftext|>"

done
