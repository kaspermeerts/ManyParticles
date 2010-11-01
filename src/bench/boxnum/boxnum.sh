maxtime=100
maxiter=500

radius=$1
worldsize=$2
maxpart=$3
#maxboxnum=$(echo scale=0; $worldsize / (2 * $radius) | bc)
maxboxnum=$4

for npart in $(seq 50 50 $maxpart)
do
	list=""
	for nbox in $(seq 7 1 $maxboxnum)
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
