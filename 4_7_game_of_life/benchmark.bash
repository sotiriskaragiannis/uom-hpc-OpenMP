#!/bin/bash

# Compile both versions
gcc -O3 game_of_life_seq.c -o game_of_life_seq.out -fopenmp
gcc -O3 game_of_life_par.c -o game_of_life_par.out -fopenmp

# Define number of threads
threads=(2 4 8 12)
output_file="results.txt"
csv_file="results.csv"

# Clear previous results
echo "Game of Life Experiment Results" > $output_file
echo "================================" >> $output_file

# Prepare CSV header
echo "Version,Threads,Execution_Time(s)" > $csv_file

# Run sequential version
echo "Running Game of Life sequential..." | tee -a $output_file
for i in {1..5}; do
    result=$(./game_of_life_seq.out)
    echo "$result" >> $output_file
    # Extract the time (assumes the output contains "Calculated in X seconds")
    time=$(echo "$result" | grep -oP 'in \K[0-9.]+')
    echo "sequential,1,$time" >> $csv_file
done

echo "--------------------------------" >> $output_file

# Run parallel version with different thread counts
for t in "${threads[@]}"; do
    echo "Running Game of Life parallel with $t threads..." | tee -a $output_file
    for i in {1..5}; do
        result=$(./game_of_life_par.out $t)
        echo "$result" >> $output_file
        # Extract the time from the result and save in CSV
        time=$(echo "$result" | grep -oP 'in \K[0-9.]+')
        echo "parallel,$t,$time" >> $csv_file
    done
    echo "--------------------------------" >> $output_file
done

echo "Experiment completed. Results saved in $output_file and $csv_file."
