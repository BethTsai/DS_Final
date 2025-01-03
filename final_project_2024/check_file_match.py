def compare_files(file1_path, file2_path):
    try:
        with open(file1_path, 'r') as file1, open(file2_path, 'r') as file2:
            # Read all lines from both files
            lines1 = file1.readlines()
            lines2 = file2.readlines()
            
            # Get the maximum length to handle files of different sizes
            max_length = max(len(lines1), len(lines2))
            
            # Initialize mismatch counter
            mismatches_found = False
            
            # Compare lines
            for i in range(max_length):
                # Get lines or empty string if file is shorter
                line1 = lines1[i].strip() if i < len(lines1) else ""
                line2 = lines2[i].strip() if i < len(lines2) else ""
                
                if line1 != line2:
                    print(f"Mismatch at line {i + 1}:")
                    print(f"File 1: {line1}")
                    print(f"File 2: {line2}")
                    print("-" * 50)
                    mismatches_found = True
            
            if not mismatches_found:
                print("Files are identical!")
                
    except FileNotFoundError:
        print("Error: One or both files not found")
    except Exception as e:
        print(f"An error occurred: {str(e)}")

if __name__ == "__main__":
    file1_path = "output_more.txt"
    file2_path = "query_more_output.txt"
    compare_files(file1_path, file2_path)