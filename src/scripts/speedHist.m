function [x, y] = speedHist(world)
	v = sqrt( world(:,5).^2 + world(:,6).^2 + world(:,7).^2 );
	[y, x] = hist(v, 50);
	width = x(2) - x(1);
	scale = sum(y*width);
	y = y / scale;
endfunction
