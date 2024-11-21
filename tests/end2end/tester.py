import subprocess
import os
import re

def run_test(algorithm_binary, test_file_path):
    try:
        result = subprocess.run(
            [algorithm_binary, test_file_path],
            capture_output=True,
            text=True,
            timeout=10,
            check=True,
        )
        output = result.stdout.strip()
        match = re.search(r"Hits:\s*(\d+)", output)
        if match:
            return int(match.group(1))
        else:
            return -1

    except subprocess.CalledProcessError as e:
        print(f"Error running {algorithm_binary} on {test_file_path}: Return code {e.returncode}, {e.stderr}")
        return -1
    except subprocess.TimeoutExpired:
        print(f"Timeout running {algorithm_binary} on {test_file_path}")
        return -1
    except Exception as e:
        print(f"An unexpected error occurred: {e}")
        return -1


def run_tests(lirs_binary, perfect_cache_binary, test_files_dir):
    test_files = [
        os.path.join(test_files_dir, f)
        for f in os.listdir(test_files_dir)
        if os.path.isfile(os.path.join(test_files_dir, f))
    ]

    results = {
        "lirs": {},
        "perfect_cache": {},
    }

    for test_file in test_files:
        results["lirs"][test_file] = run_test(lirs_binary, test_file)
        results["perfect_cache"][test_file] = run_test(perfect_cache_binary, test_file)

    return results


def main():
    test_files_path = "/mnt/c/c++/vladimirov_course/tests/end2end"

    lirs_path = "/mnt/c/c++/vladimirov_course/build/tests/lirs"
    perfect_cache_path = "/mnt/c/c++/vladimirov_course/build/tests/perfect_cache"

    all_results = run_tests(lirs_path, perfect_cache_path, test_files_path)

    print("Test Results:")
    for algorithm, results_dict in all_results.items():
        print(f"\nAlgorithm: {algorithm}")
        for test_file, hits in results_dict.items():
            print(f"  {test_file}: Hits = {hits if hits != -1 else 'Error'}")

if __name__ == "__main__":
    main()

