#set key outside
#set output 'xyz.png'


set terminal png size 1366,768
set output 'xyz.png'
#set terminal svg size 1366,768
#set output 'graph.svg'

set key autotitle columnheader

plot for [col=2:9] 'data' using 1:col with lines title col lw 2

