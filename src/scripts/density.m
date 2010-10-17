load "densdata.csv";

[f,r] = hist(densdata, 200);
rho = f./r.^2;
plot(r, rho);
