function makeGraph(name,destdir,relImgDir,xlab,ylab,ylabrule,width,height);
	xlabel(xlab);
	ylabel(['\rule{0pt}{',ylabrule,'}',ylab]);

	system(['mkdir ',destdir,' &>/dev/null']);
	print([destdir,'/',name,'.tex'],'-depslatex',['-S',width,',',height]);
	system(['cd ',destdir,'; epstopdf ',name,'.eps; rm ',name,'.eps']);
	system(['cd ',destdir,"; sed -e 's#",destdir,'#',relImgDir,"#' ", name,".tex > ",name,"-fixed.tex; mv ",name,"-fixed.tex ", name,".tex"]);
endfunction

