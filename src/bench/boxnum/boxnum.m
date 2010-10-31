addpath('../../scripts/octave');
destdir = "../../../latex/images";

b = load("boxnum1.csv");
b = [b;load("boxnum2.csv")];
b = [b;load("boxnum3.csv")];
b = [b;load("boxnum4.csv")];

npart = b(:,1);
nbox = b(:,2) .^ 3;
ips = b(:,3);

msPerIter = 1000 ./ ips;

fit = polyfit(npart, msPerIter, 1);
plot(npart, msPerIter, "o", npart, polyval(fit, npart));

name='linearComplexity';
ylabrule='0.3cm';
xlab='Number of particles ($n$)';
ylab='Milliseconds per iteration';
width='700';
height='500';
makeGraph(name,destdir,xlab,ylab,ylabrule,width,height);


plot(npart, nbox, "o");

name='idealNbox';
ylabrule='0.3cm';
xlab='Number of particles ($n$)';
ylab='Ideal number of boxes';
width='700';
height='500';
makeGraph(name,destdir,xlab,ylab,ylabrule,width,height);


