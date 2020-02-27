

for i in 105 116 132 142 161 176 210 246 300 346
do
	echo "seed $seed"

	python ./run_generation.py --model_type=gpt2 --length=20 --model_name_or_path=gpt2-xl --out_file=wholeCloth_${seed}__300.txt --in_file=wholeClothInput.txt --gen_words=300 --seed=$seed --stop_token="<|endoftext|>"

done
