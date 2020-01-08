# pipe in list of chapter names

# pipe out text

stripSubheadingsArg=""

if [[ $1 == "-stripSubHeadings" ]]; then
	stripSubheadingsArg="-stripSubHeadings"
fi

parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

while read -r line; 
do 
	cat $line | sed -e 's/ — /—/g' | sed -e '/^$/d' | sed -e 's/\(Chapter [0-9]*\)/\n\n\n\1\n\n/' | sed -e 's/Prologue/\n\n\nPrologue\n\n/' | sed -e 's/Epilogue/\n\n\nEpilogue\n\n/' |  sed -e "s/END OF CHAPTER/END OF CHAPTER\n/" | $parent_path/processChapter.sh $stripSubheadingsArg | sed -e "s/END OF CHAPTER/\f/"

done

echo -n -e "\n\n\nThe End\n"