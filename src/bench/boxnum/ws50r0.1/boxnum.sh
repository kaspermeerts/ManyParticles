radius=0.1
worldsize=50
maxpart=10000
maxboxnum=$(echo "scale=0; $worldsize / (2 * $radius)" | bc)

cd ..
./boxnum.sh $radius $worldsize $maxpart $maxboxnum
