#/bin/bash

# This script exercises the real workload
npages=1500000
clock=200

for trace in trace.1 trace.2
do
    mkdir -p output/$trace
    
    #for alg in fifo lru nru aging second-chance
    for alg in random fifo second-chance #nru second-chance lru
    do
        # numero de espaços nas memória
        for nframes in 4 8 16 32
        do
        printf "running $alg\n"
        python memory_simulation.py $npages $nframes $alg $clock < load/$trace.in > output/$trace/$trace.$alg.$nframes.out
    done
    done
done