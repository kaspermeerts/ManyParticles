numData = 17;
data = 0:4:64;
bins = linspace(0, 2, 20);
[x, y] = meshgrid(bins, data);

for i=1:numData
	file = sprintf("relax%02d00.csv", data(i));
	world = load(file);
	v = sqrt(world(:,5).^2 + world(:,6).^2 + world(:,7).^2);
	t = hist(v, bins);
	z(i,:) = min(t, 200);
end

surface(x, y, z);
