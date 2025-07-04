# Terminal de sortie (image PNG)
set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'comp_perf_plot.png'

# Titre et axes
set title "Tracé du temps (en secondes) pris pour additionner deux tableau de taille x via avx et sse4.2"
set xlabel "x"
set ylabel "y"

# Grille
set grid

# Légende
set key outside top right

# Tracé à partir du fichier
plot 'data.txt' using 1:2 title "avx" with lines linewidth 2 linecolor rgb "blue", \
     'data.txt' using 1:3 title "sse4.2" with lines linewidth 2 linecolor rgb "red"
