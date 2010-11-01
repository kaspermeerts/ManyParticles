maxtime=200
maxiter=1000

radius=0.1
worldsize=50
maxboxnum=50

npart=10000

for nbox in $(seq 1 1 200)
do
	boxsize=$(echo "$worldsize / $nbox" | bc -l)
	output=$(../../main $boxsize $nbox $npart $radius -i $maxiter -b $maxtime)
	echo "$nbox $output";
done
