ifndef CXX
CXX = c++
endif
CFLAGS = -std=c++14 -Wall

wade: main.cc
	$(CXX) $(CFLAGS) $<  -o $@

test: wade
	(wc -l $(TEST) | cut -f1 -d' ' ; cat $(TEST)) | ./wade
	dot -Tpdf graph.dot -o ~/srv/graph.pdf

.PHONY: test

#(wc -l $1 | cut -f1 -d' ' ; cat $1 ) | ./a.out

#dot -Tpdf graph.dot -o ~/srv/graph.pdf
