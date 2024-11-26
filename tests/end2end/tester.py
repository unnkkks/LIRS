import subprocess
import os


LIRS_FILE_PATH: str = "/mnt/c/c++/vladimirov_course/build/tests/lirs"
PERFECT_CACHE_FILE_PATH: str = "/mnt/c/c++/vladimirov_course/build/tests/perfect_cache"
TEST_DIRECTORY: str = "/mnt/c/c++/vladimirov_course/tests/end2end"
LIRS_OUTPUT_FILE_PATH: str = "lirs_results.txt"
PERFECT_CACHE_OUTPUT_FILE_PATH: str = "perfect_cache_results.txt"


def run_tests(binary_path: str, test_dir: str, output_file: str) -> None:
    with open(output_file, "w") as outfile:
        for filename in os.listdir(test_dir):
            test_filepath = os.path.join(test_dir, filename)
            try:
                with open(test_filepath, "r") as testfile:
                    lines = testfile.readlines()
                    capacity, num_keys = map(int, lines[0].strip().split())
                    outfile.write(f"Test file: {filename} (Capacity: {capacity}, Keys: {num_keys})\n")

                    print(f"Current test '{filename}' in process...")
                    result = subprocess.run([binary_path], input='\n'.join(lines), text=True, capture_output=True)
                    outfile.write(f"  Result: {result.stdout}\n")
                    outfile.flush()
                    print(f"Test '{filename}' end.\n")
            except Exception as exc:
                print(f"Error with '{filename}': {exc}")


def main() -> None:
    run_tests(LIRS_FILE_PATH, TEST_DIRECTORY, LIRS_OUTPUT_FILE_PATH)
    run_tests(PERFECT_CACHE_FILE_PATH, TEST_DIRECTORY, PERFECT_CACHE_OUTPUT_FILE_PATH)

    print(f"Results written to {LIRS_OUTPUT_FILE_PATH} and {PERFECT_CACHE_OUTPUT_FILE_PATH}")


if __name__ == "__main__":
    main()
