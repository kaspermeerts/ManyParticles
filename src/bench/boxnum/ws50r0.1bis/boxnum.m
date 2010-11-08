addpath('../../../scripts/octave');
destdir = "../../../../latex/images";
relImgDir = "images";

b = load("ws50r0.1-1");
b = [b;load("ws50r0.1-2")];
b = [b;load("ws50r0.1-3")];
b = [b;load("ws50r0.1-4")];
b = [b;load("ws50r0.1-5")];
b = [b;load("ws50r0.1-6")];


b = load("ws50r0.1-step1");
b = [b;load("ws50r0.1-step2")];
b = [b;load("ws50r0.1-step3")];
b = [b;load("ws50r0.1-step4")];
b = [b;load("ws50r0.1-step5")];

npart = b(:,1);
nbox = b(:,2) .^ 3;
ips = b(:,3);

sPerIter = 1 ./ ips;
msPerIter = 1000 ./ ips;

plot(npart / 1000, msPerIter, "o");
%axis([0,1,0,4],'autox');

name='linearComplexityR0p1-1M';
ylabrule='0.3cm';
xlab='Number of particles $\times$ 1000';
ylab='Milliseconds per iteration';
width='700';
height='500';
makeGraph(name,destdir,relImgDir,xlab,ylab,ylabrule,width,height);


plot(npart / 1000, nbox / 1000, "o");

name='idealNboxR0p1-1M';
ylabrule='0.3cm';
xlab='Number of particles $\times$ 1000';
ylab='Ideal number of boxes $\times$ 1000';
width='700';
height='500';
makeGraph(name,destdir,relImgDir,xlab,ylab,ylabrule,width,height);


