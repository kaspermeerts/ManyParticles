addpath('../../scripts/octave');
destdir = "../../../latex/images";
relImgDir = "images";

clf; hold on;

b = load("rad0.0001.csv");
nbox = b(:,1);
ips = b(:,2);
plot(nbox, ips, "ok");

b = load("rad0.01.csv");
nbox = b(:,1);
ips = b(:,2);
plot(nbox, ips, "ob");

b = load("rad0.1.csv");
nbox = b(:,1);
ips = b(:,2);
plot(nbox, ips, "og");

b = load("rad0.5.csv");
nbox = b(:,1);
ips = b(:,2);
plot(nbox, ips, "or");




clf;

b = load("rad0.1.csv");
nbox = b(:,1);
ips = b(:,2);
h = plot(nbox, ips, "o");
set(h, "linewidth", 2, "markersize", 5);

name='fixedPartnum';
ylabrule='0.3cm';
xlab='Number of boxes in each dimension';
ylab='Iterations per second';
width='700';
height='500';
makeGraph(name,destdir,relImgDir,xlab,ylab,ylabrule,width,height);


hold off;
