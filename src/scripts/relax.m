addpath("octave");
destdir = "../../latex/images";
relImgDir = "images";

a1 = load("relax1-1.csv");
a2 = load("relax1-2.csv");
a3 = load("relax1-3.csv");
b1 = load("relax2-1.csv");
b2 = load("relax2-2.csv");
b3 = load("relax2-3.csv");
c1 = load("relax3-1.csv");
c2 = load("relax3-2.csv");
c3 = load("relax3-3.csv");

[x1, y1] = speedHist([a1;a2;a3]);
[x2, y2] = speedHist([b1;b2;b3]);
[x3, y3] = speedHist([c1;c2;c3]);

x = linspace(0, max([x1,x2,x3]), 50);
h = plot(x1, y1, 'o-', x2, y2, 'o-', x3, y3, 'o-', x, maxwellDist(1/3,x), 'k');
axis([0 2.5 0 2]);
set(h, "linewidth", 2);
set(h, "markersize", 10);

name='relax';
ylabrule='0.3cm';
xlab='Speed';
ylab='Probability density';
width='700';
height='500';
makeGraph(name,destdir,relImgDir,xlab,ylab,ylabrule,width,height);
