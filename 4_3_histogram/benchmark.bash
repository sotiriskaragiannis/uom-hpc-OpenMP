#!/bin/bash

# Compile both versions
gcc -O3 histogram_seq.c -o histogram_seq.out -fopenmp
gcc -O3 histogram_par.c -o histogram_par.out -fopenmp

# Define number of threads for parallel execution
threads=(2 4 8 12)
output_file="results.txt"
csv_file="results.csv"

# Input image file
input_image="original.bmp"

# Ensure the input image exists
if [ ! -f "$input_image" ]; then
    echo "Error: Input image '$input_image' not found!"
    exit 1
fi

# Clear previous results
echo "Histogram Calculation Experiment Results" > $output_file
echo "=========================================" >> $output_file
echo "Version,Threads,Execution_Time(s)" > $csv_file  # Add CSV header

# Run sequential version and save results to CSV
echo "Running Histogram Sequential..." | tee -a $output_file
for i in {1..5}; do
    result=$(./histogram_seq.out)
    echo "$result" >> $output_file
    time=$(echo "$result" | grep -oP '\d+\.\d+')  # Extract time
    echo "sequential,1,$time" >> $csv_file
done

echo "--------------------------------" >> $output_file

# Run parallel version with different thread counts and save to CSV
for t in "${threads[@]}"; do
    echo "Running Histogram Parallel with $t threads..." | tee -a $output_file
    for i in {1..5}; do
        result=$(./histogram_par.out $t)
        echo "$result" >> $output_file
        time=$(echo "$result" | grep -oP '\d+\.\d+')  # Extract time
        echo "parallel,$t,$time" >> $csv_file
    done
    echo "--------------------------------" >> $output_file
done

echo "Experiment completed. Results saved in $output_file and $csv_file."
