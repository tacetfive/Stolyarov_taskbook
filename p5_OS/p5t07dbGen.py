from datetime import datetime
import numpy as np
import argparse
import linecache # module for random access to specific lines from a text file
import sys

file_words = "test_files/words.txt"
FILE_LENGTH = 466_434
COUNTER_SIZE = 4 # uint32_t
KEY_SIZE = 59 # except \0 terminator

parser = argparse.ArgumentParser(description="This script generate simple \
database which records\ncontain two fields: \
counter (32bit integer) and\nkey (60-symbols string)",
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
group  = parser.add_mutually_exclusive_group(required=False)
group.add_argument("-r", action="store_true", 
                    help="generate random 60-bytes length strings (for very large DB)")
group.add_argument("-w", action="store_true", 
                    help="generate db using words from file")

parser.add_argument("records", type=int, nargs='?', default=1, 
                    help="number of records to generate")
parser.add_argument("filename", type=str, nargs='?', 
                    default="test_files/db"+datetime.now().strftime("%y%m%d%H%M%S"),
                    help="name of database file")

args = parser.parse_args()

def print_to_file(filename, record_counters, keys):
    if len(record_counters) != len(keys):
        print("Error: len(record_counters) is not equal len(keys)")
        print(len(record_counters))
        print(len(keys))
        sys.exit(1)
    #for c, k in zip(record_counters, keys):
    #    print(c)
    #    print(k)

    with open(filename, "wb") as file:
        for c, k in zip(record_counters, keys):
            file.write(c)
            file.write(k.ljust(60, '\0').encode("ascii"))

def create_db_words(records_quantity):
    if records_quantity > FILE_LENGTH:
        print("Error: can not create database with such size.")
        sys.exit(1)
    record_counters = generate_numbers_list(records_quantity)
    # generate numbers of lines which will the key of database records
    rng = np.random.default_rng()
    lines_numbers = rng.choice(FILE_LENGTH, records_quantity, replace=False)
    # rstrip method truncates symbols out of string
    keys = [linecache.getline(file_words, ln).rstrip('\n') \
             for ln in lines_numbers]
    linecache.clearcache()
    if any(not key.strip() for key in keys):
        print("empty or whitespace string detected. Exit with error.")
        sys.exit(1)
    print_to_file(args.filename, record_counters, keys)

def generate_numbers_list(records_quantity): # non unique 32bit numbers for record
    rng = np.random.default_rng()
    generated_list = rng.integers(low=1, high=200, size=records_quantity, \
                                  dtype=np.uint32)
    return generated_list

def create_db_random_str(): # for database with enormous number of records
    print_to_file(args.filename, keys)



if not args.r and not args.w:
    create_db_words(10)
elif args.r:
    create_db_random_str()
else:
    create_db_words(args.records)

