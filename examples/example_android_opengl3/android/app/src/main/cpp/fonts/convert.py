import os
import struct
import zlib
import glob
import argparse

def binary_to_compressed_c(filename, use_compression, use_static):
    try:
        # Read the font file into a binary blob
        with open(filename, 'rb') as f:
            font_data = f.read()

        # Get the file size
        file_size = len(font_data)

        # Compress the data if needed
        if use_compression:
            compressed_data = zlib.compress(font_data)
        else:
            compressed_data = font_data

        # Replace hyphens with underscores in the base file name for the header file
        base_filename = os.path.splitext(os.path.basename(filename))[0]
        base_filename_for_header = base_filename.replace('-', '_')  # This is the one used for the header

        # Create a C-style header output
        output_filename = os.path.splitext(filename)[0] + '.hpp'

        with open(output_filename, 'w') as output_file:
            # Write the header comment with the file name and size
            output_file.write(f"// File: '{filename}' ({file_size} bytes)\n")
            output_file.write(f"// Exported using binary_to_compressed_c.exe -u32 \"{filename}\" {base_filename_for_header}\n\n")
            
            # Generate the static const variables and the data array
            compressed_size = len(compressed_data)
            num_elements = (compressed_size + 3) // 4  # Ensure the array size is a multiple of 4

            output_file.write(f"static const unsigned int {base_filename_for_header}_compressed_size = {compressed_size};\n")
            output_file.write(f"static const unsigned int {base_filename_for_header}_compressed_data[{num_elements}] = {{\n")
            
            # Write the compressed data in hex format
            for i in range(0, len(compressed_data), 4):
                # Extract the next 4 bytes (32 bits) and unpack them
                chunk = compressed_data[i:i+4]
                if len(chunk) < 4:
                    # Pad the chunk with zero bytes if it is less than 4 bytes (for the last chunk)
                    chunk = chunk + b'\x00' * (4 - len(chunk))
                # Unpack the chunk into an unsigned integer (4 bytes)
                data = struct.unpack('I', chunk)[0]
                # Write the data in hex format
                output_file.write(f"    0x{data:08X},\n")
            
            # Close the array
            output_file.write("};\n")

        print(f"Output written to {output_filename}")
        return base_filename  # Ensure this is always returned

    except Exception as e:
        print(f"Error processing file {filename}: {e}")
        return None  # Return None if an error occurs

def process_files(pattern, use_compression, use_static):
    # List to hold the base file names for later use
    font_base_names = []
    
    # Use glob to match files based on the pattern
    for filename in glob.glob(pattern):
        print(f"Processing file: {filename}")
        base_filename = binary_to_compressed_c(filename, use_compression, use_static)
        
        # Check if base_filename is not None before appending to the list
        if base_filename is not None:
            font_base_names.append(base_filename)
        else:
            print(f"Warning: Failed to process {filename}")
    
    return font_base_names  # Return the list of base file names

def generate_load_function(font_base_names):
    # Sort the font base names alphabetically
    font_base_names.sort()

    # Generate the LoadFontsFromEmbeddedData function
    function_code = """#include <cstring> // For strncpy
    
// Function to load fonts from the embedded compressed data
void LoadFontsFromEmbeddedData()
{
    ImFontConfig font_config;

    // Set oversampling for better quality
    font_config.OversampleH = 4;  // Higher value means better horizontal text quality
    font_config.OversampleV = 4;  // Higher value means better vertical text quality
    font_config.PixelSnapH = true; // Align pixels to the grid for sharper text
"""
    
    # Add the code to load each font
    for base_filename in font_base_names:
        # Format font name with spaces (e.g., "Afacadflux Bold")
        font_name = base_filename.replace('_', ' ').title().replace(' ', ' ')  # Replace underscores with spaces and capitalize
        base_filename_for_use = base_filename.replace('-', '_')  # Ensure underscores are used for C++ variable names
        function_code += f"""
    // {font_name} Fonts
    strncpy(font_config.Name, \"{font_name}\", sizeof(font_config.Name) - 1);
    font_config.Name[sizeof(font_config.Name) - 1] = '\\0';
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF({base_filename_for_use}_compressed_data, {base_filename_for_use}_compressed_size, 23.0f, &font_config);
"""
    
    # Finalize the function
    function_code += """
    // Finalizing font load
    ImGui::GetIO().Fonts->Build();
}
"""
    # Write the function to a file
    with open("LoadFontsFromEmbeddedData.hpp", "w") as f:
        f.write(function_code)
    
    print("LoadFontsFromEmbeddedData function generated and saved to 'LoadFontsFromEmbeddedData.hpp'.")

def generate_font_includes(font_base_names):
    # Sort the font base names alphabetically
    font_base_names.sort()

    # Generate the font_includes.hpp file that includes all the header files
    include_code = """// File: font_includes.hpp
// Automatically generated includes for embedded font headers
"""
    
    # Add #include for each base filename (do not modify base_filename here)
    for base_filename in font_base_names:
        include_code += f"#include \"{base_filename}.hpp\"\n"
    
    # Write the include file
    with open("font_includes.hpp", "w") as f:
        f.write(include_code)

    print("font_includes.hpp file generated and saved to 'font_includes.hpp'.")

def main():
    # Create the parser for CLI arguments
    parser = argparse.ArgumentParser(description="Compress font files and generate C header files")
    
    # Add arguments
    parser.add_argument("pattern", type=str, help="Pattern for font files (e.g., '*.ttf')")
    
    # Parse the arguments
    args = parser.parse_args()
    
    # Hardcoded values for compress and static
    use_compression = True
    use_static = True
    
    # Process the files based on the provided pattern
    font_base_names = process_files(args.pattern, use_compression, use_static)
    
    # Generate the LoadFontsFromEmbeddedData function and the font_includes.hpp file
    generate_load_function(font_base_names)
    generate_font_includes(font_base_names)

if __name__ == "__main__":
    main()
