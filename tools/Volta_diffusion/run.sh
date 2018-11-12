#!/bin/bash
nodes=1
ppn=1
let nmpi=$nodes*$ppn
#--------------------------------------
cat >batch.job <<EOF
#BSUB -o %J.out
#BSUB -e %J.err
#BSUB -R "span[ptile=${ppn}]"
#BSUB -R "affinity[core(5):distribute=pack]"
#BSUB -n ${nmpi}
#BSUB -x
#BSUB -G guests
#BSUB -q pdebug
#BSUB -W 15
#---------------------------------------
export BIND_THREADS=yes
export OMP_NUM_THREADS=1
ulimit -s 10240

#cuda-memcheck ./diff.x
nvidia-smi -q -d CLOCK
mpirun -mxm -np ${nmpi}  nvprof -m all ./diff.x 
mpirun -mxm -np ${nmpi}  nvprof ./diff.x 
mpirun -mxm -np ${nmpi}  nvprof  --print-gpu-trace ./diff.x 
#mpirun -mxm -np ${nmpi} ../../bin/cardioid-ray-openmp 
#mpirun -np 1 nvprof -o cardioid-cuda-tl.%q{OMPI_COMM_WORLD_RANK}.nvprof ../../bin/cardioid-ray-openmp 
EOF
#---------------------------------------
bsub < batch.job