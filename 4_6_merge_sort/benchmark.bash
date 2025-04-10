#!/bin/bash

# Compile both versions
gcc -O3 mergesort_seq.c -o mergesort_seq.out -fopenmp
gcc -O3 mergesort_par.c -o mergesort_par.out -fopenmp

# Define problem sizes and thread counts
mergesort_sizes=(10000 100000 1000000 10000000)
threads=(2 4 8 12)
output_file="results.txt"
csv_file="results.csv"

# Clear previous results
echo "Mergesort Experiment Results" > $output_file
echo "================================" >> $output_file

# Prepare CSV header
echo "Version,Size,Threads,Execution_Time(s)" > $csv_file

# Run sequential mergesort experiments and save results to CSV
for size in "${mergesort_sizes[@]}"; do
    echo "Running mergesort sequential for size $size..." | tee -a $output_file
    for i in {1..5}; do
        result=$(./mergesort_seq.out $size)
        echo "$result" >> $output_file
        # Extract the time (assuming "Calculated in X seconds" output format)
        time=$(echo "$result" | grep -oP 'in \K[0-9.]+')
        echo "sequential,$size,1,$time" >> $csv_file
    done
    echo "--------------------------------" >> $output_file
done

# Run parallel mergesort experiments and save results to CSV
for size in "${mergesort_sizes[@]}"; do
    for t in "${threads[@]}"; do
        echo "Running mergesort parallel with $t threads for size $size..." | tee -a $output_file
        for i in {1..5}; do
            result=$(./mergesort_par.out $size $t)
            echo "$result" >> $output_file
            # Extract the time and save it in CSV format
            time=$(echo "$result" | grep -oP 'in \K[0-9.]+')
            echo "parallel,$size,$t,$time" >> $csv_file
        done
    done
    echo "--------------------------------" >> $output_file
done

echo "Experiment completed. Results saved in $output_file and $csv_file."
