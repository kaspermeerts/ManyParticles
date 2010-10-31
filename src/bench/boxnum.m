b = load("boxnum.csv");
n = b(:,1);
t = b(:,2:end);
[_, ix] = min(t');
ix = ix .+ 15;
plot(n, ix);
