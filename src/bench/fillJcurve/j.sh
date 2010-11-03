for r in $(seq 0.01 0.01 2)
do
	for i in $(seq 1 10)
	do
		echo "$r $(../../main 4 5 180 $r -f -i 0)"
	done
done
