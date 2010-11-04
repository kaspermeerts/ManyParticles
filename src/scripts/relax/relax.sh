for i in $(seq 0 4 64)
do
	iterations=$(( $i * 100))
	filename=$(printf "relax%02d00.csv" $i)
	../../main 1.5 20 1000 0.2 -i $iterations -t 0.05 -d > $filename
done
