#!/bin/bash

# Compile both versions
gcc -O3 count_sort_seq.c -o count_sort_seq.out -fopenmp
gcc -O3 count_sort_par.c -o count_sort_par.out -fopenmp

# Define problem sizes and thread counts
problem_sizes=(25000 50000 100000 200000)
threads=(2 4 8 12)
output_file="results.txt"
csv_file="results.csv"

# Clear previous results
echo "Count Sort Experiment Results" > $output_file
echo "================================" >> $output_file
echo "Version,Threads,Size,Execution_Time(s)" > $csv_file  # CSV header

# Run sequential and save results to CSV
for size in "${problem_sizes[@]}"; do
    echo "Running sequential for size $size..." | tee -a $output_file
    for i in {1..5}; do
        result=$(./count_sort_seq.out $size)
        echo "$result" >> $output_file
        time=$(echo "$result" | grep -oP '\d+\.\d+')  # Extract execution time
        echo "sequential,1,$size,$time" >> $csv_file
    done

    # Run parallel with different thread counts and save to CSV
    for t in "${threads[@]}"; do
        echo "Running parallel with $t threads for size $size..." | tee -a $output_file
        for i in {1..5}; do
            result=$(./count_sort_par.out $size $t)
            echo "$result" >> $output_file
            time=$(echo "$result" | grep -oP '\d+\.\d+')  # Extract execution time
            echo "parallel,$t,$size,$time" >> $csv_file
        done
    done

    echo "--------------------------------" >> $output_file
done

echo "Experiment completed. Results saved in $output_file and $csv_file."
