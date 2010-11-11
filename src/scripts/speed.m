addpath('octave');
destdir = "../../latex/images";
relImgDir = "images";

world1 = load("world1.csv");
world2 = load("world2.csv");
world3 = load("world3.csv");

[x1, y1] = speedHist(world1);
[x2, y2] = speedHist(world2);
[x3, y3] = speedHist(world3);

[x , y ] = speedHist([world1; world2; world3]);


T = 1/3;
xt = linspace(0,max(x),100);
yt = maxwellDist(T, xt);
h = plot(x, y, 'o', xt, yt);
set(h, "linewidth", 2);
set(h, "markersize", 10);
axis([0,2.5,0,1.2]);

name='maxwell';
ylabrule='0.3cm';
xlab='Speed';
ylab='Probability density';
width='700';
height='500';
makeGraph(name,destdir,relImgDir,xlab,ylab,ylabrule,width,height);
