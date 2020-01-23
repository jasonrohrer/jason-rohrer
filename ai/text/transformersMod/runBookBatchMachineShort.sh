rm outRunChapters.txt

unbuffer bash ./runChapters.sh 2 17 42 earlSamsInput.txt 1 requiredWord.txt 2000 3150 --allow_chapter_sub_headings=0 2>&1 | tee outRunChapters.txt