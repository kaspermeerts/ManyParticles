addpath('../../../scripts/octave');
destdir = "../../../../latex/images";
relImgDir = "images";

b = load("ws50r0.1-1.csv");

npart = b(:,1);
nbox = b(:,2) .^ 3;
ips = b(:,3);

secPerIter = 1 ./ ips;

fit = polyfit(npart, secPerIter, 1);
plot(npart, secPerIter, "o", npart, polyval(fit, npart));
axis([0,1,0,4],'autox');

name='linearComplexity2';
ylabrule='0.3cm';
xlab='Number of particles ($n$)';
ylab='Seconds per iteration';
width='700';
height='500';
makeGraph(name,destdir,relImgDir,xlab,ylab,ylabrule,width,height);


plot(npart, nbox, "o");

name='idealNbox2';
ylabrule='0.3cm';
xlab='Number of particles ($n$)';
ylab='Ideal number of boxes';
width='700';
height='500';
makeGraph(name,destdir,relImgDir,xlab,ylab,ylabrule,width,height);


