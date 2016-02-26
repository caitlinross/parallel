set term po eps enhanced color
set autoscale xfix
set autoscale yfix
set cbrange [0:1]
set output "gol-128-100-0-16.eps"
plot 'gol-128-100-0-16.dat' matrix with image notitle

set output "gol-128-100-25-16.eps"
plot 'gol-128-100-25-16.dat' matrix with image notitle

set output "gol-128-100-50-16.eps"
plot 'gol-128-100-50-16.dat' matrix with image notitle

set output "gol-128-100-75-16.eps"
plot 'gol-128-100-75-16.dat' matrix with image notitle

set output "gol-128-100-90-16.eps"
plot 'gol-128-100-90-16.dat' matrix with image notitle

set xlabel "MPI Rank"
set ylabel "Execution time"
set xrange[0:17]
set yrange[0:9]
set title "Scaling Study for Game of Life program"
set output "scaling.eps"
plot "scaling.dat" using 1:2:xtic(1) with linespoints notitle
