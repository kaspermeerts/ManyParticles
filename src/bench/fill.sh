for i in $(seq 0 50 10000)
do
	echo -en "$i\t"
	../main 1 1 $i 0.01 -i 1 -b
done
