set term po eps enhanced color
set autoscale xfix
set autoscale yfix
set cbrange [0:1]
set xlabel "Number of MPI Ranks"
set ylabel "Execution time (in seconds)"
#set xrange[0:17]
#set yrange[0:9]
set logscale x
set logscale y
set title "Scaling Study for Game of Life program"
set output "scaling-64.eps"
plot "scaling64-0.dat" using ($1*$2):4:xtic(5) with linespoints linecolor rgb "blue" title "0% threshold", \
    "scaling64-25.dat" using ($1*$2):4 with linespoints linecolor rgb "orange" title "25% threshold", \
    "scaling64-50.dat" using ($1*$2):4 with linespoints linecolor rgb "green" title "50% threshold", \
    "scaling64-75.dat" using ($1*$2):4 with linespoints linecolor rgb "red" title "75% threshold"

set output "scaling-t50.eps"
plot "scaling32-50.dat" using ($1*$2):4:xtic(5) with linespoints linecolor rgb "blue" title "32 tasks/node", \
    "scaling64-50.dat" using ($1*$2):4:xtic(5) with linespoints linecolor rgb "red" title "64 tasks/node", \

set xlabel "Number of Nodes"
set output "scaling-t50-2.eps"
plot "scaling32-50.dat" using 2:4 with linespoints linecolor rgb "blue" title "32 tasks/node", \
    "scaling64-50.dat" using 2:4:xtic(2) with linespoints linecolor rgb "red" title "64 tasks/node", \

set title "Speedup for Game of Life program"
set xlabel "Number of MPI Ranks"
set ylabel "Speedup"
set output "speedup.eps"
plot "scaling64-0-speedup.dat" using 1:2:xtic(1) with linespoints linecolor rgb "blue" title "0% threshold", \
    "scaling64-25-speedup.dat" using 1:2 with linespoints linecolor rgb "orange" title "25% threshold", \
    "scaling64-50-speedup.dat" using 1:2 with linespoints linecolor rgb "green" title "50% threshold", \
    "scaling64-75-speedup.dat" using 1:2 with linespoints linecolor rgb "red" title "75% threshold"
