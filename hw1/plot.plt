set term po eps enhanced color
set autoscale xfix
set autoscale yfix
set cbrange [0:1]
set output "gol-16-100-0.eps"
plot 'gol-16-100-0.out' matrix with image notitle

set output "gol-16-100-25.eps"
plot 'gol-16-100-25.out' matrix with image notitle

set output "gol-16-100-50.eps"
plot 'gol-16-100-50.out' matrix with image notitle

set output "gol-16-100-75.eps"
plot 'gol-16-100-75.out' matrix with image notitle

set output "gol-16-100-90.eps"
plot 'gol-16-100-90.out' matrix with image notitle
