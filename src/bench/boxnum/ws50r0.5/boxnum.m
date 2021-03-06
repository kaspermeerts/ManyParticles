addpath('../../../scripts/octave');
destdir = "../../../../latex/images";
relImgDir = "images";

b = load("boxnum1.csv");
b = [b;load("boxnum2.csv")];
b = [b;load("boxnum3.csv")];
b = [b;load("boxnum4.csv")];

npart = b(:,1);
nbox = b(:,2) .^ 3;
ips = b(:,3);

msPerIter = 1000 ./ ips;

fit = polyfit(npart, msPerIter, 1);
plot(npart / 1000, msPerIter, "o", npart / 1000, polyval(fit, npart));
axis([0,1,0,4],'autox');

name='linearComplexityR0p5';
ylabrule='0.3cm';
xlab='Number of particles $\times$ 1000';
ylab='Milliseconds per iteration';
width='700';
height='500';
makeGraph(name,destdir,relImgDir,xlab,ylab,ylabrule,width,height);


plot(npart / 1000, nbox / 1000, "o");

name='idealNboxR0p5';
ylabrule='0.3cm';
xlab='Number of particles $\times$ 1000';
ylab='Ideal number of boxes $\times$ 1000';
width='700';
height='500';
makeGraph(name,destdir,relImgDir,xlab,ylab,ylabrule,width,height);


