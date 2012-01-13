reset
#set term pdf font ",5" 
#set output "richards_benchmarks.pdf"
set terminal postscript eps enhanced solid color
set output "richards_benchmark.eps"
set title "Richards Benchmark"
set datafile separator "," #csv is comma separated
set yrange [0:]      #plot starting from 0
set ylabel "Average execution time (ms)"
set style data histograms  #plot histogram style
set style fill solid 1.00 border 0 #fill bars
set style histogram errorbars gap 2 lw 1
set key left
plot "benchmark_results/generational_nocache_noTagging_richards.csv" using 2:3:xtic(1) ti "SOM++ generational", \
     "benchmark_results/generational_nocache_tagging_richards.csv" using 2:3 ti "SOM++ generational (tagging)", \
     "benchmark_results/copying_nocache_noTagging_richards.csv" using 2:3 ti "SOM++ copying", \
     "benchmark_results/copying_nocache_tagging_richards.csv" using 2:3 ti "SOM++ copying (tagging)", \
     "benchmark_results/mark_sweep_nocache_noTagging_richards.csv" using 2:3 ti "SOM++ mark-sweep", \
     "benchmark_results/mark_sweep_nocache_tagging_richards.csv" using 2:3 ti "SOM++ mark-sweep (tagging)"