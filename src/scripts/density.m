Nbins = 500;
Nstr = "1000";
bins = linspace(0, 8, Nbins)';

N = str2double(Nstr);
data = zeros(N-1, 1);
f = zeros(Nbins, 1);
rho = zeros(Nbins, 1);

for i= 1:50
	[_, out] = system(["../main 2 4 ", Nstr, " 0.3"]);
	data = str2double(out);
	f = hist(data, bins)';
	rho = rho + f./bins.^2;
end

plot(bins, rho)
