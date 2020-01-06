rm outRunChapters.txt

unbuffer bash ./runChapters.sh 1 25 42 ozChapters 1 requiredWordOz.txt 1000 2000 --allow_chapter_sub_headings=1 2>&1 | tee outRunChapters.txt