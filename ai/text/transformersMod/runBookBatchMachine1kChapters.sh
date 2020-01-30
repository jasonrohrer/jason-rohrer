rm outRunChapters.txt

unbuffer bash ./runChapters.sh 2 46 42 earlSamsInput.txt 1 requiredWord.txt 700 1200 --allow_chapter_sub_headings=0 2>&1 | tee outRunChapters.txt