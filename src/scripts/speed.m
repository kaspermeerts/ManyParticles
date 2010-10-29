load world.csv

T = 1/3;

v = sqrt( world(:,5).^2 + world(:,6).^2 + world(:,7).^2 );
[y, x] = hist(v,50);
x1 = linspace(0,max(v),100);
y1 = sqrt(2/pi/T^3).*x1.^2.*exp(-x1.^2/2/T);
width = x(2)-x(1);
scale = sum(y*width);
plot(x,y/scale,x1,y1);
