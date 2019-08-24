
x = 10
y = True

def func(x):
    """
    docstring?
    """
    print(x)

def add(a, b):
    result = a + b

    print("%d", result)


if y:
    func("test")
else:
    print("%d", x)


# add two numbers and print
add(30, 55)