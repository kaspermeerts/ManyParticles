function makeGraph(name,destdir,xlab,ylab,ylabrule,width,height);
	xlabel(xlab);
	ylabel(['\rule{0pt}{',ylabrule,'}',ylab]);

	system(['mkdir ',destdir,' &>/dev/null']);
	print([destdir,'/',name,'.tex'],'-depslatex',['-S',width,',',height]);
	system(['cd ',destdir,'; epstopdf ',name,'.eps; rm ',name,'.eps']);
endfunction

