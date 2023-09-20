# coding: utf8

import argparse
import sqlite3
import json

# Import the db_fetch_json function if it's defined in a separate module

def db_fetch_json(db_path, output_file_path, required_tables):
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()
    required_data = {}

    for table in required_tables:
        cursor.execute(f'SELECT * FROM {table}')
        data = cursor.fetchall()
        columns = [description[0] for description in cursor.description]    # Get column names from the cursor description for JSON keys
        required_data[table]  = [dict(zip(columns, row)) for row in data]   # Create a list of dictionaries (each row is a dictionary)
    conn.close()

    required_data_json = json.dumps(required_data, indent=4)

    if output_file_path:
        with open(output_file_path, 'w') as file:
            file.write(required_data_json)

    return required_data_json

def main():
    parser = argparse.ArgumentParser(description="My Program Description")
    parser.add_argument("--db", required=True, help="Input file path")
    parser.add_argument("--output", help="Output file path")
    parser.add_argument("--table", nargs="*", choices=["MAC_UE", "RLC_bearer", "PDCP_bearer", "SLICE", "UE_SLICE", "GTP_NGUT", "KPM_MeasRecord", "KPM_LabelInfo", "all"], required=True, help="which table want to fetch")
    #MAC -- MAC_UE
    #RLC -- RLC_bearer
    #PDCP -- PDCP_bearer
    #SLICE -- SLICE
    #UE_SLICE -- UE_SLICE
    #GTP -- GTP_NGUT
    #KPM_K -- KPM_MeasRecord
    #KPM_L -- KPM_LabelInfo

    args = parser.parse_args()
    db_path = args.db
    output_file_path = args.output
    required_tables = args.table
   
    return db_fetch_json(db_path, output_file_path, required_tables)

if __name__ == "__main__":
    main()
