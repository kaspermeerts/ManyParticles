radius=0.1
worldsize=50
cd ..

minpart=" 25000"
partstep="25000"
maxpart="100000"
minboxnum=49
boxnumstep=1
maxboxnum=110
./boxnum.sh $radius $worldsize $minpart $partstep $maxpart $minboxnum $boxnumstep $maxboxnum


minpart=" 150000"
partstep="050000"
maxpart=" 500000"
minboxnum=100
boxnumstep=1
maxboxnum=200
./boxnum.sh $radius $worldsize $minpart $partstep $maxpart $minboxnum $boxnumstep $maxboxnum

minpart="  550000"
partstep=" 050000"
maxpart=" 1200000"
minboxnum=160
boxnumstep=1
maxboxnum=240
./boxnum.sh $radius $worldsize $minpart $partstep $maxpart $minboxnum $boxnumstep $maxboxnum
