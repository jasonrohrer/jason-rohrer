echo "Genning $1 words of titles with seed $2"

python ./run_generation.py --model_type=gpt2 --length=20 --model_name_or_path=gpt2-xl --seed=$2 --num_samples=20 --gen_min_words=$1 --prompt="Fans around the word were delighted by the release of the first parody novel written entirely by an artificial intelligence, based on J. K. Rowling's ever-popular \"Harry Potter\" series. The novel, titled, \"Harry Potter and the"