import argparse
import pandas as pd   # Import pandas with an alias 'pd'
import json

def json_xlsx(mode, input_file_path, output_file_path):
    if mode == "xlsx_to_json":
        excel_file = pd.ExcelFile(input_file_path)
        sheet_names = excel_file.sheet_names
        data_frames = {}
        for sheet_name in sheet_names:
            df = excel_file.parse(sheet_name)
            data_frames[sheet_name] = df.to_dict(orient='records')
        with open(output_file_path, 'w') as file:
            json.dump(data_frames,file, indent=4)
        return data_frames
    elif mode == "json_to_xlsx":
        with open(input_file_path, 'r') as file:
            data = json.load(file)
        with pd.ExcelWriter(output_file_path, engine='openpyxl') as writer:
            for sheet_name, sheet_data in data.items():
                df = pd.DataFrame(sheet_data)
                df.to_excel(writer, sheet_name=sheet_name, index=False, index_label=None)
    return

def main():
    # Create an ArgumentParser object
    parser = argparse.ArgumentParser(description="My Program Description")

    # Define command-line arguments
    parser.add_argument("--input", required=True, help="Input file path")
    parser.add_argument("--output", help="Output file path")
    parser.add_argument("--mode", choices=["xlsx_to_json", "json_to_xlsx"], required=True, help="Mode of operation")

    # Parse the command-line arguments
    args = parser.parse_args()

    # Access the parsed arguments
    input_file_path = args.input
    output_file_path = args.output
    mode = args.mode

    # Perform the job based on the provided parameters
    return json_xlsx(mode, input_file_path, output_file_path)

if __name__ == "__main__":
    main()

