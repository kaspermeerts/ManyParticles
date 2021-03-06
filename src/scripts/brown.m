addpath('octave');
destdir = "../../latex/images";
relImgDir = "images";

b1 = load("brown1.csv");
b2 = load("brown2.csv");
b3 = load("brown3.csv");
b4 = load("brown4.csv");

plot3(
	b1(:,1), b1(:,2), b1(:,3),'r', "linewidth", 2,
	b2(:,1), b2(:,2), b2(:,3),'g', "linewidth", 2,
	b3(:,1), b3(:,2), b3(:,3),'b', "linewidth", 2,
	b4(:,1), b4(:,2), b4(:,3),'y', "linewidth", 2,
	b1(1,1), b1(1,2), b1(1,3),'ok', "markersize", 10, "linewidth", 20);

axis([-10 330 -10 330 -10 330]);

name='brown';
ylabrule='0cm';
xlab='';
ylab='';
width='1000';
height='800';
makeGraph(name,destdir,relImgDir,xlab,ylab,ylabrule,width,height);
