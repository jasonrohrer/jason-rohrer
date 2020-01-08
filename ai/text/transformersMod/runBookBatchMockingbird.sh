rm outRunChapters.txt

unbuffer bash ./runChapters.sh 1 31 42 mockingbirdChapters 1 requiredWordMockingbird.txt 2000 3000 --allow_chapter_sub_headings=1 2>&1 | tee outRunChapters.txt