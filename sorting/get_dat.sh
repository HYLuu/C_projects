#!/bin/bash


for i in {10..10000..10}
    do
	    ./sorting -s -p 0 -n $i >> s.dat
	    ./sorting -h -p 0 -n $i >> h.dat
	    ./sorting -q -p 0 -n $i >> q.dat
	    ./sorting -b -p 0 -n $i >> b.dat
    done

awk ' {print $3, $5} ' s.dat > shell.dat
awk ' {print $3, $5} ' h.dat > heap.dat
awk ' {print $3, $5} ' q.dat > quick.dat
awk ' {print $3, $5} ' b.dat > batcher.dat

gnuplot <<END
    set terminal pdf
    set output "move_rev.pdf"
    set ylabel "Number of Moves"
    set xlabel "Number of Elements"
    set logscale y 2
    plot "shell.dat" with lines title "Shell Sort" lt rgb "#CC99FF", \
        "heap.dat" with lines title "Heap Sort" lt rgb "#FFB2666", \
        "batcher.dat" with lines title "Batcher Sort" lt rgb "#00CC66", \
        "quick.dat" with lines title "Quick Sort" lt rgb "#3399FF"
END

rm *.dat
