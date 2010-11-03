TARGET=particle

all: tex

ALL: 
	make -B all
	make -B bib
	make -B all
	make -B all

tex:
	pdflatex ${TARGET}.tex
	rm ${TARGET}.log

bib:
	bibtex ${TARGET}

clean:
	rm -f ${TARGET}.{log,aux,bbl,blg}

