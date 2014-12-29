load 'common-cas.plt'

set title "n = 8"
set xlabel "k"

set logscale y
set logscale y2

set yrange [1:]

set terminal postscript eps enhanced
set output "n8kid-cas.eps"

plot \
     "n8kid.dat" using 9:25 title "PS0" pt 4,\
     "n8kid.dat" using 9:29 title "PW0" pt 6,\
     "n8kid.dat" using 9:17 title "PS1" pt 12,\
     "n8kid.dat" using 9:21 title "PW1" pt 14,\
     "n8kid.dat" using 9:33 title "HS" pt 1,\
     "n8kid.dat" using 9:37 title "HW" pt 2
set output
