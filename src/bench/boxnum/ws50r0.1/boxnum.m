addpath('../../../scripts/octave');
destdir = "../../../../latex/images";
relImgDir = "images";

b = load("ws50r0.1-1");
b = [b;load("ws50r0.1-2")];

npart = b(:,1);
nbox = b(:,2) .^ 3;
ips = b(:,3);

msPerIter = 1000 ./ ips;

plot(npart / 1000, msPerIter, "o");
%axis([0,1,0,4],'autox');

name='linearComplexityR0p1';
ylabrule='0.3cm';
xlab='Number of particles $\times$ 1000)';
ylab='Milliseconds per iteration';
width='700';
height='500';
makeGraph(name,destdir,relImgDir,xlab,ylab,ylabrule,width,height);



plot(npart / 1000, nbox / 1000, "o");

name='idealNboxR0p1';
ylabrule='0.3cm';
xlab='Number of particles $\times$ 1000';
ylab='Ideal number of boxes $\times$ 1000';
width='700';
height='500';
makeGraph(name,destdir,relImgDir,xlab,ylab,ylabrule,width,height);


