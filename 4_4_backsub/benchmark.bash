#!/bin/bash

# Compile both versions
gcc -O3 backsub_seq.c -o backsub_seq.out -fopenmp
gcc -O3 backsub_par.c -o backsub_par.out -fopenmp

# Define problem sizes and thread counts
backsub_sizes=(1000 5000 10000 20000)
threads=(2 4 8 12)
output_file="results.txt"
csv_file="results.csv"

# Clear previous results
echo "Backsubstitution Experiment Results" > $output_file
echo "================================" >> $output_file
echo "Version,Threads,Size,Execution_Time(s)" > $csv_file  # CSV header

# Run backsubstitution sequential experiments and save to CSV
for size in "${backsub_sizes[@]}"; do
    echo "Running backsub sequential for size $size..." | tee -a $output_file
    for i in {1..5}; do
        result=$(./backsub_seq.out $size)
        echo "$result" >> $output_file
        time=$(echo "$result" | grep -oP '\d+\.\d+')  # Extract the time
        echo "sequential,1,$size,$time" >> $csv_file
    done

    # Run backsubstitution parallel experiments with different thread counts
    for t in "${threads[@]}"; do
        echo "Running backsub parallel with $t threads for size $size..." | tee -a $output_file
        for i in {1..5}; do
            result=$(./backsub_par.out $size $t)
            echo "$result" >> $output_file
            time=$(echo "$result" | grep -oP '\d+\.\d+')  # Extract the time
            echo "parallel,$t,$size,$time" >> $csv_file
        done
    done

    echo "--------------------------------" >> $output_file
done

echo "Experiment completed. Results saved in $output_file and $csv_file."
