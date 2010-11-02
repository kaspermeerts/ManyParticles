data = ["relax0000.csv"; "relax0100.csv"; "relax0200.csv"; "relax0300.csv"; "relax0400.csv"; "relax0500.csv"; "relax0600.csv"; "relax0700.csv"; "relax0800.csv"; 
	"relax1600.csv"; "relax3200.csv"; "relax6400.csv"];
numData = 12;
bins = linspace(0, 2, 50);
[x, y] = meshgrid(bins, 1:numData);

for i=1:numData
	file = data(i, :);
	world = load(data(i,:));
	v = sqrt(world(:,5).^2 + world(:,6).^2 + world(:,7).^2);
	t = hist(v, bins);
	z(i,:) = min(t, 100);
end

surface(x, y, z);
