import random
import string

import sys


def generate_test_file(filename="input.txt", num_entries=1000):
    with open(filename, "w") as f:
        for _ in range(num_entries):
            # Generate a 10-char key
            key = "".join(random.choices(string.ascii_letters, k=10))
            # Generate a random integer value
            value = random.randint(0, 100000)
            f.write(f"{key} {value}\n")
    print(f"Test input file '{filename}' generated with {
          num_entries} entries.")


if __name__ == "__main__":
    generate_test_file(num_entries=int(sys.argv[1]))
