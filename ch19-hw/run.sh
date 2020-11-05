#!/usr/bin/env bash
set -euo pipefail

power_pages=12
num_tries=100000
pages=2
let max_pages=pages**power_pages
outfile=results.txt

[ -f tlb ] || clang tlb.c -o tlb -O3 -Wall -Wextra

generate_data () {
    echo "Testing up to $max_pages ($pages^$power_pages)"
    [ -f $outfile ] && mv $outfile $outfile.backup
    while [ $pages -le $max_pages ]; do
        echo -n "Testing $pages pages..."
        res=`./tlb $pages $num_tries -s`
        echo "$pages $res" >> "$outfile"
        let pages=pages*2
        echo "done"
    done
    echo "done"
}

generate_graph () {
    echo -n "Generating plot..."
    gnuplot -e "set terminal png size 800,600; \
        set output \"results.png\"; \
        set logscale x; \
        set xlabel \"# of Pages Accesed\"; \
        set ylabel \"Milliseconds per access\"; \
        plot \"results.txt\""
    echo "done"
}

generate_data
generate_graph
