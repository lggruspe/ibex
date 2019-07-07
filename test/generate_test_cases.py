import random

def empty():
    return ''

def number():
    digits = "0123456789"
    def integer():
        n = random.randint(1, 9)
        if n == 1:
            return random.choice(digits)

        rv = random.choice(digits[1:])
        for i in range(1, n):
            rv += random.choice(digits)
        return rv

    rv = integer()
    if random.randint(0, 1):
        rv += "."
        for _ in range(random.randint(1, 9)):
            rv += random.choice(digits)
    if random.randint(0, 1):
        rv += random.choice("eE")
        if random.randint(0, 1):
            rv += random.choice("+-")
        rv += integer()
    return rv

def whitespace():
    return random.choice([' ', "\\t", "\\n"])

def identifier():
    A = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
    B = "0123456789"
    rv = random.choice(A)
    for _ in range(random.randint(0, 15)):
        rv += random.choice(A + B)
    return rv

generators = [empty, number, whitespace, identifier]

def everything_but(A, a):
    return [b for b in A if b != a]

def create_test_case_generator(instance_generator, other_instance_generators):
    def test_case_generator(n, p):
        cases = []
        for _ in range(n):
            if random.random() < p:
                cases.append((instance_generator(), True))
            else:
                false_generator = random.choice(other_instance_generators)
                cases.append((false_generator(), False))
        return cases
    return test_case_generator

def create_test_case_generators(instance_generators):
    test_case_generators = []
    for instance_generator in instance_generators:
        test_case_generator = create_test_case_generator(instance_generator,
                everything_but(instance_generators, instance_generator))
        test_case_generators.append(test_case_generator)

    return test_case_generators

def create_test_cases(instance_generators, n=30, p=0.5):
    test_case_generators = create_test_case_generators(instance_generators)
    test_cases = [gen(n, p) for gen in test_case_generators]
    return test_cases

def print_test_cases(instance_generators):
    test_cases = create_test_cases(instance_generators)

    for gen, test_case in zip(instance_generators, test_cases):
        with open("tests/" + gen.__name__ + ".csv", "w") as f:
            for instance, label in test_case:
                f.write("{},{}\n".format(instance, int(label)))

print_test_cases(generators)
