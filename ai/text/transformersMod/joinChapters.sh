# pipe in list of chapter names

# pipe out text

while read -r line; 
do 
	cat $line | sed -e 's/ — /—/g' | sed -e "s/END OF CHAPTER/\f/" | sed -e '/^$/d' | sed -e 's/\(Chapter [0-9]*\)/\n\n\n\n\1\n\n/' | sed -e 's/Prologue/\n\n\n\n\Prologue\n\n/' 
done

echo -n -e "\n\n\n\nThe End\n"