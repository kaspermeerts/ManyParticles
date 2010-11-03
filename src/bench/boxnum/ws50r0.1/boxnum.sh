radius=0.1
worldsize=50
cd ..

#minpart=50
#partstep=50
#maxpart=1000
#minboxnum=2
#boxnumstep=1
#maxboxnum=30
#./boxnum.sh $radius $worldsize $minpart $partstep $maxpart $minboxnum $boxnumstep $maxboxnum

minpart=1000
partstep=1000
maxpart=10000
minboxnum=24
boxnumstep=1
maxboxnum=60
./boxnum.sh $radius $worldsize $minpart $partstep $maxpart $minboxnum $boxnumstep $maxboxnum

minpart=10000
partstep=1000
maxpart=100000
minboxnum=49
boxnumstep=1
maxboxnum=102
./boxnum.sh $radius $worldsize $minpart $partstep $maxpart $minboxnum $boxnumstep $maxboxnum

