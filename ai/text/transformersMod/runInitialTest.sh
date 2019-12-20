
# run from inside
# source activate pytorch_p36

conda install -c eumetsat expect

pip install --upgrade pip

pip install transformers

git clone https://github.com/huggingface/transformers.git

git clone https://github.com/jasonrohrer/jason-rohrer.git

cd jason-rohrer/ai/text/transformersMod


# run short one as a warm-up (to get model cached)
python ./run_generation.py --model_type=gpt2 --length=20 --model_name_or_path=gpt2-xl --out_file=out.txt --in_file=sampleInputShort.txt --gen_words=50

rm out.txt

# run longer one and time it
time python ./run_generation.py --model_type=gpt2 --length=20 --model_name_or_path=gpt2-xl --out_file=out.txt --in_file=sampleInputShort.txt --gen_words=500
