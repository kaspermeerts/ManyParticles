maxtime=200
maxiter=1000

radius=0.1
worldsize=50
maxboxnum=50

boxsize=2
nbox=25

for npart in $(seq 100 100 10000)
do
	output=$(../../main $boxsize $nbox $npart $radius -i $maxiter -b $maxtime)
	echo "$npart $output";
done
