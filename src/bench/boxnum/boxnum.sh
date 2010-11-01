maxtime=100
maxiter=500

radius=$1
worldsize=$2

minpart=$3
partstep=$4
maxpart=$5

minboxnum=$6
boxnumstep=$7
maxboxnum=$8

for npart in $(seq $minpart $partstep $maxpart)
do
	list=""
	for nbox in $(seq $minboxnum $boxnumstep $maxboxnum)
	do
		boxsize=$(./calc $worldsize / $nbox)
		output=$(../../main $boxsize $nbox $npart $radius -i $maxiter -b $maxtime)
		list="$list\n$output $nbox"
	done
	best=$(echo -e $list | sort -n | tail -n 1)
	ips=$(echo $best | cut -d ' ' -f 1)
	bestboxnum=$(echo $best | cut -d ' ' -f 2)
	
	echo "$npart $bestboxnum $ips"
	if [ $bestboxnum == $maxboxnum ]; then
		echo "bestboxnum is maxboxnum!" >&2
	fi

done
