addpath('../../scripts/octave');
destdir = "../../../latex/images";
relImgDir = "images";

b = load("singlebox.csv");

npart = b(:,1);
ips = b(:,2);

msPerIter = 1000 ./ ips;

fit = polyfit(npart, msPerIter, 2);
plot(npart, msPerIter, "o", npart, polyval(fit, npart));
axis([0 1 0 12000],'autox');

name='quadraticComplexity';
ylabrule='0.3cm';
xlab='Number of particles ($n$)';
ylab='Milliseconds per iteration';
width='700';
height='500';
makeGraph(name,destdir,relImgDir,xlab,ylab,ylabrule,width,height);
