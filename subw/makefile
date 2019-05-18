word="aabbab"
fn=subw
eps=${fn}.eps
pdf=${fn}.pdf
png=${fn}.png
dot=${fn}.dot
tsk=${fn}.tsk
source=${fn}.cpp

draw: dot
	xdot ${dot} &

eps: ${eps}

${eps}: ${dot}
	dot -Teps -o $@ ${dot}

pdf: ${pdf}

${pdf}: ${dot}
	dot -Tpdf -o $@ ${dot}
	
png: ${png}

${png}: ${dot}
	dot -Tpng -o $@ ${dot}


dot: ${dot}

${dot}: ${tsk} makefile
	./$< ${word} > $@

${tsk}: ${source}
	g++ -Wall -O2 -ggdb -o$@ $<

clean:
	rm -f ${tsk} ${dot} ${pdf} ${eps}
