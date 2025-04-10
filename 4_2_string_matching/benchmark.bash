#!/bin/bash

# Compile both versions
gcc -O3 string_matching_seq.c -o string_matching_seq.out -fopenmp
gcc -O3 string_matching_par.c -o string_matching_par.out -fopenmp

# Define number of threads
threads=(2 4 8 12)
output_file="results.txt"
csv_file="results.csv"

# Input file and pattern to search
input_file="file.txt"
search_pattern="test"

# Clear previous results
echo "String Matching Experiment Results" > $output_file
echo "================================" >> $output_file
echo "Version,Threads,Execution_Time(s)" > $csv_file  # CSV header

# Run sequential version and write results to CSV
echo "Running String Matching sequential..." | tee -a $output_file
for i in {1..5}; do
    result=$(./string_matching_seq.out "$input_file" "$search_pattern")
    echo "$result" >> $output_file
    time=$(echo "$result" | grep -oP '\d+\.\d+')  # Extract time
    echo "sequential,1,$time" >> $csv_file
done

echo "--------------------------------" >> $output_file

# Run parallel version with different thread counts and write to CSV
for t in "${threads[@]}"; do
    echo "Running String Matching parallel with $t threads..." | tee -a $output_file
    for i in {1..5}; do
        result=$(./string_matching_par.out "$input_file" "$search_pattern" $t)
        echo "$result" >> $output_file
        time=$(echo "$result" | grep -oP '\d+\.\d+')  # Extract time
        echo "parallel,$t,$time" >> $csv_file
    done
    echo "--------------------------------" >> $output_file
done

echo "Experiment completed. Results saved in $output_file and $csv_file."
