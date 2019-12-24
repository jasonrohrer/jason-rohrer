rm outRunChapters.txt

unbuffer bash ./runChapters.sh 2 25 42 earlSamsInput.txt 1 requiredWord.txt 2>&1 | tee outRunChapters.txt