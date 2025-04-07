import os
import csv
import matplotlib.pyplot as plt
import numpy as np

# Extracting csv's values
def calculate_column_averages(csv_path):
    column_sums = []
    column_counts = []
    
    with open(csv_path, 'r') as file:
        csv_reader = csv.reader(file)
        
        for row in csv_reader:
            # Convert all values to float
            try:
                numeric_row = [float(value) for value in row if value.strip()]
            except ValueError:
                # Skip row if conversion fails
                continue
            
            # Update column sums and counts
            for i, value in enumerate(numeric_row):
                # Extend lists if needed
                while len(column_sums) <= i:
                    column_sums.append(0)
                    column_counts.append(0)
                
                column_sums[i] += value
                column_counts[i] += 1
    
    # Calculate averages
    column_averages = [
        sum_val / count if count > 0 else None 
        for sum_val, count in zip(column_sums, column_counts)
    ]
    
    return column_averages

if __name__ == "__main__":
    originalPath = os.path.join(os.path.dirname(__file__), "csv/original_times.csv")
    refactoredPath = os.path.join(os.path.dirname(__file__), "csv/refactored_times.csv")
                                  
    originalValues = calculate_column_averages(originalPath)
    refactoredValues = calculate_column_averages(refactoredPath)
    
    # Find the length of the longest list
    max_length = max(len(originalValues), len(refactoredValues))
    
    # Pad the shorter list with zeros
    originalValues.extend([0] * (max_length - len(originalValues)))
    refactoredValues.extend([0] * (max_length - len(refactoredValues)))
    
    x1 = np.arange(len(originalValues))
    x2 = np.arange(len(refactoredValues))
    
    # Create figure and axis
    fig, ax = plt.subplots(figsize=(10, 6))
    
    # Plot the data
    ax.plot(x1, originalValues, 'b-o', label='Original')
    ax.plot(x2, refactoredValues, 'r-s', label='Refactored')
    
    # Add labels for axes
    ax.set_xlabel('Move number (per player)')
    ax.set_ylabel('Average iteration speed')
    
    # Add grid
    ax.grid(True, linestyle='--', alpha=0.7)
    
    # Add legend
    ax.legend()
    
    # Show the plot
    plt.tight_layout()
    plt.show()
