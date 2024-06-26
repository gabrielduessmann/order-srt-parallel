import matplotlib.pyplot as plt
import numpy as np

# Function to read data from a file and return it as a list of floats
def read_data(file_path):
    with open(file_path, 'r') as file:
        data = [float(line.strip()) for line in file]
    return data

# Function to calculate the average of a list of numbers
def calculate_average(data):
    return np.mean(data)

# Function to generate and save a line graph
def generate_line_graph(x_values, y_values, output_file):
    plt.figure()
    plt.plot(x_values, y_values, marker='o', linestyle='-', color='b')
    plt.xlabel('Data Points')
    plt.ylabel('Average Time (seconds)')
    plt.title('Average Time vs Data Points')
    plt.grid(True)
    plt.savefig(output_file)
    plt.close()

def main():
    # List of input files and their corresponding X values
    input_files = ["results_order-seq.o_generated_subtitles_1M.srt.txt",
                   "results_order-seq.o_generated_subtitles_5M.srt.txt",
                   "results_order-seq.o_generated_subtitles_10M.srt.txt",
                   "results_order-seq.o_generated_subtitles_20M.srt.txt",
                   "results_order-seq.o_generated_subtitles_50M.srt.txt"]
    x_values = ["1M", "5M", "10M", "20M", "50M"]

    # Calculate the average Y values for each file
    y_values = []
    for file in input_files:
        data = read_data(file)
        avg = calculate_average(data)
        y_values.append(avg)

    # Generate and save the line graph
    output_file = "average_time_graph.pdf"
    generate_line_graph(x_values, y_values, output_file)
    print(f"Graph saved as {output_file}")

if __name__ == "__main__":
    main()
