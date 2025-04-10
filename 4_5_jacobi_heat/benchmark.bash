#!/bin/bash

# Compile both versions
gcc -O3 jacobi_heat_seq.c -o jacobi_heat_seq.out -lm -fopenmp
gcc -O3 jacobi_heat_par.c -o jacobi_heat_par.out -lm -fopenmp

# Define number of threads
threads=(2 4 8 12)
output_file="results.txt"
csv_file="results.csv"

# Clear previous results
echo "Experiment Results" > $output_file
echo "================================" >> $output_file

# Prepare CSV header
echo "Version,Threads,Execution_Time(s)" > $csv_file

# Run Jacobi Heat sequential and save correct results to CSV
echo "Running Jacobi Heat sequential..." | tee -a $output_file
for i in {1..5}; do
    result=$(./jacobi_heat_seq.out)                       # Run the sequential program
    echo "$result" >> $output_file                        # Save full output to text file
    time=$(echo "$result" | grep -oP '\d+\.\d+')          # Extract execution time (float number)
    echo "sequential,1,$time" >> $csv_file                # Append only time to CSV
done
echo "--------------------------------" >> $output_file

# Run Jacobi Heat parallel with different thread counts and save to CSV
for t in "${threads[@]}"; do
    echo "Running Jacobi Heat parallel with $t threads..." | tee -a $output_file
    for i in {1..5}; do
        result=$(./jacobi_heat_par.out $t)
        echo "$result" >> $output_file
        time=$(echo "$result" | grep -oP '\d+\.\d+')      # Extract only time from output
        echo "parallel,$t,$time" >> $csv_file             # Correct CSV line with time
    done
    echo "--------------------------------" >> $output_file
done

echo "Experiment completed. Results saved in $output_file and $csv_file."
