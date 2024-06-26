import matplotlib.pyplot as plt

# Function to generate the graph with multiple lines
def generate_graph(x_labels, y_values_list, labels, output_file):
    plt.figure()
    
    for y_values, label in zip(y_values_list, labels):
        plt.plot(x_labels, y_values, marker='o', linestyle='-', label=label)
    
    plt.xlabel('Quantidade')
    plt.ylabel('Média (s)')
    plt.title('Ordenação quicksort')
    plt.legend()
    plt.grid(True)
    plt.xticks(rotation=45)
    plt.tight_layout()
    plt.savefig(output_file)
    plt.close()

    print(f"Graph saved as {output_file}")

def main():
    # Provide the X-axis labels and corresponding average times for each case
    x_labels = ['500K', '1M', '5M', '10M', '20M']
    y_values_case1 = [0.4399724, 0.96551915, 5.5261884, 11.69259195, 24.25771295]   # Average time sequential
    y_values_case2 = [0.16850225, 0.38450375, 2.2465457000000003, 4.7318555, 9.626512400000001]  # Average time OpenMP
    #y_values_case1 = [0.15811335, 0.3127835, 1.56890265, 3.1129827, 6.22960905] # read
    #y_values_case2 = [0.12718875, 0.259186, 1.3214217, 2.635698, 5.21615825] # write
    # Combine all y-values into a list
    y_values_list = [y_values_case1, y_values_case2]
    
    # Labels for each line
    labels = ['Sequencial', 'OpenMP']

    # Output file for the graph
    output_file = 'average_time.pdf'
    
    # Generate the graph
    generate_graph(x_labels, y_values_list, labels, output_file)

if __name__ == "__main__":
    main()
