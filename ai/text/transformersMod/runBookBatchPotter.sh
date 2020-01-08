rm outRunChapters.txt

unbuffer bash ./runChapters.sh 1 17 42 potterChapters 1 requiredWordPotter.txt 2000 3000 --allow_chapter_sub_headings=1 2>&1 | tee outRunChapters.txt