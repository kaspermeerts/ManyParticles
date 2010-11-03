addpath('../../scripts/octave');
destdir = "../../../latex/images";
relImgDir = "images";

data = load("J1");
data = [data; load("J2")];
data = [data; load("J3")];

rs = data(:,1);
fails = data(:,2);

npart = 180;
Vworld = 20^3;
Vpart = npart * 4*pi/3 * rs.^3;

Vfrac = Vpart / Vworld;
failsFrac = fails / npart;

%plot(rs, fails, "o");
semilogy(Vfrac, failsFrac, "o");
axis([0,0.63,1,2], 'autoy');

name='fillJcurve';
ylabrule='0.9cm';
xlab='Volume fraction of particles in world';
ylab='Fraction of failed particle placements';
width='700';
height='500';
makeGraph(name,destdir,relImgDir,xlab,ylab,ylabrule,width,height);
