rm outInterleaved.txt

python ./run_interleavedGeneration.py --model_type=gpt2 --length=20 --model_name_or_path=gpt2-xl --out_file=outInterleaved.txt --in_file=oz.txt --seed=42 