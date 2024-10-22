import random

tests_number = 10

for test_number in range(tests_number):
    name_of_file = "test_" + f'{test_number + 1:02}' + ".txt"

    file = open(name_of_file, 'w')

    cache_capacity = random.randint(1000, 2000)

    elems_number = random.randint(1000000, 2000000)

    test_text = str(cache_capacity) + " " + str(elems_number) + "\n"

    for elem_number in range(elems_number):
        if elem_number % 10 == 0:
            file.write(test_text)
            test_text = ""
        test_text += str(int(random.gauss(100000, 10000))) + " "
    file.write(test_text)
    file.close()
