#!/bin/bash

(wc -l $1 | cut -f1 -d' ' ; cat $1 ) | ./a.out 

dot -Tpdf graph.dot -o ~/srv/graph.pdf
