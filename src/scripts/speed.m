load world1.csv
load world2.csv
%load world3.csv

[x1, y1] = speedHist(world1);
[x2, y2] = speedHist(world2);
[x3, y3] = speedHist(world3);

[x , y ] = speedHist([world1; world2; world3]);


T = 1/3;
xt = linspace(0,max(x),100);
yt = sqrt(2/pi/T^3).*xt.^2.*exp(-xt.^2/2/T);
plot(x1,y1,'or', x2, y2, 'og', x3, y3, 'ob', x, y, '-k', xt,yt);
