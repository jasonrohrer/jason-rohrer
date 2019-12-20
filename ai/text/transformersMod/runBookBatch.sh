rm outRunChapters.txt

stdbuf -o0 ./runChapters.sh 2 10 42 earlSamsInput.txt 3 requiredWord.txt 2>&1 | tee outRunChapters.txt