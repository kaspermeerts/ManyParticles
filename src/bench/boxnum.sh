for i in $(seq 10 5 90)
do
	n=$(./calc $i ^ 2)
	echo -en "$n\t"

	for j in $(seq 15 1 40)
	do
		size=$(./calc 32 / $j)
		t=$(./timer timeout 7 ../main $size $j $n 0.7 -i 500)
		echo -en "$t\t"
	done
	echo ""
done
