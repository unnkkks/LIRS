import subprocess
import os
import re

test_files_path = "/mnt/c/c++/vladimirov_course/tests/end2end"

lirs_cpp_path = "/mnt/c/c++/vladimirov_course/tests/src/lirs.cpp"
perfect_cache_cpp_path = "/mnt/c/c++/vladimirov_course/tests/src/perfect_cache.cpp"

def run_test(algorithm_path, test_file_path):

  process = subprocess.run(
    ["./a.out", algorithm_path, test_file_path],
    stdout=subprocess.PIPE,
    stderr=subprocess.PIPE,
    cwd=os.path.dirname(algorithm_path)
  )

  output = process.stdout.decode("utf-8")
  match = re.search(r"Hits: (\d+)", output)
  if match:
    return int(match.group(1))
  else:
    return 0

def compile_cpp(cpp_file_path):
  process = subprocess.run(
    ["g++", "-o", "a.out", cpp_file_path],
    cwd=os.path.dirname(cpp_file_path)
  )
  return process.returncode == 0


algorithms = {
  "LIRS": lirs_cpp_path,
  "Perfect Cache": perfect_cache_cpp_path
}


results = {}

for algorithm_name, algorithm_path in algorithms.items():
  if compile_cpp(algorithm_path):
    results[algorithm_name] = {}
  else:
    print(f"Ошибка компиляции алгоритма {algorithm_name}")

for test_file in os.listdir(test_files_path):
  test_file_path = os.path.join(test_files_path, test_file)
  if os.path.isfile(test_file_path):
    for algorithm_name, algorithm_path in algorithms.items():
      if algorithm_name in results:
        hits = run_test(algorithm_path, test_file_path)
        results[algorithm_name][test_file] = hits

print("Результаты тестов:")
for algorithm_name, algorithm_results in results.items():
  print(f"\n{algorithm_name}:")
  for test_file, hits in algorithm_results.items():
    print(f" {test_file}: {hits} попаданий")
