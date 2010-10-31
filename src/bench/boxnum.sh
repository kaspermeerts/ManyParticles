maxtime=100
maxiter=500

radius=0.5
worldsize=40
maxboxnum=40

for i in $(seq 5 1 100)
do
	npart=$(./calc $i ^ 2)

	list=""
	for nbox in $(seq 5 1 $maxboxnum)
	do
		boxsize=$(./calc $worldsize / $nbox)
		output=$(../main $boxsize $nbox $npart $radius -i $maxiter -b $maxtime)
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
