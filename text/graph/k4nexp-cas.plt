load 'common-cas.plt'

set title "k = 4"
set xlabel "n"

set logscale y
set logscale y2
set logscale x

set terminal postscript eps enhanced
set output "k4nexp-cas.eps"

plot \
     "k4nexp.dat" using 5:25 title "PS0" pt 4,\
     "k4nexp.dat" using 5:29 title "PW0" pt 6,\
     "k4nexp.dat" using 5:17 title "PS1" pt 12,\
     "k4nexp.dat" using 5:21 title "PW1" pt 14,\
     "k4nexp.dat" using 5:33 title "HS" pt 1,\
     "k4nexp.dat" using 5:37 title "HW" pt 2

set output
