clf; hold on;
b = load("boxnum1.csv");

npart = b(:,1);
nbox = b(:,2);
ips = b(:,3);
%plot(npart, 1 ./ ips);
plot(npart, nbox .^ 3, "o");



b = load("boxnum2.csv");

npart = b(:,1);
nbox = b(:,2);
ips = b(:,3);
plot(npart, nbox .^ 3, "o");

hold off;
