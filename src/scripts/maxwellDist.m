function f = maxwellDist(T, v)
	f = sqrt(2/pi/T^3).*v.^2.*exp(-v.^2/2/T);
endfunction
