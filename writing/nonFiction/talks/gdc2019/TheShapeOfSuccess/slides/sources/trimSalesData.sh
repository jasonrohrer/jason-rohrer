cat $1 | tail -n +5 | awk -F "," '{print $1 "," $18}' | awk -F "," '{arr[$1]+=$2} END {for (i in arr) {print i,arr[i]}}' | sort | cat -n | head -n 32