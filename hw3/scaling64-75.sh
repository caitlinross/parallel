#!/bin/bash

#SBATCH --job-name=scaling64-75
#SBATCH -t 01:00:00
#SBATCH -D /gpfs/u/home/PCP5/PCP5ctln/barn
#
#SBATCH --mail-type=ALL
#SBATCH --mail-user=rossc3@rpi.edu

srun -N1 --ntasks 64 -o scaling64-1node-t75.log --overcommit ./assignment2 8192 100 75 &
srun -N2 --ntasks 128 -o scaling64-2node-t75.log --overcommit ./assignment2 8192 100 75 &
srun -N4 --ntasks 256 -o scaling64-4node-t75.log --overcommit ./assignment2 8192 100 75 &
srun -N8 --ntasks 512 -o scaling64-8node-t75.log --overcommit ./assignment2 8192 100 75 &
srun -N16 --ntasks 1024 -o scaling64-16node-t75.log --overcommit ./assignment2 8192 100 75 &
srun -N32 --ntasks 2048 -o scaling64-32node-t75.log --overcommit ./assignment2 8192 100 75 &
srun -N64 --ntasks 4096 -o scaling64-64node-t75.log --overcommit ./assignment2 8192 100 75 &
srun -N128 --ntasks 8192 -o scaling64-128node-t75.log --overcommit ./assignment2 8192 100 75 & 
wait

