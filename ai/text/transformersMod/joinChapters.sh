# pipe in list of chapter names

# pipe out text

parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

while read -r line; 
do 
	cat $line | sed -e 's/ — /—/g' | sed -e "s/END OF CHAPTER/\f/" | sed -e '/^$/d' | sed -e 's/\(Chapter [0-9]*\)/\n\n\n\n\1\n\n/' | sed -e 's/Prologue/\n\n\n\nPrologue\n\n/' | sed -e 's/Epilogue/\n\n\n\nEpilogue\n\n/' | $parent_path/processChapter.sh

done

echo -n -e "\n\n\n\nThe End\n"