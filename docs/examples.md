# List
* [calculator](#calculator) - calculator that have all basic math operations
* [factorial](#factorial) - calculate product of all numbers from 1 to `n`


## Calculator
```clike
func main() {
    loop (true) {
        int operation;
        write("\n", "1. Addition\n", "2. Subtraction\n", "3. Multiplication\n", "4. Division\n", "5.Exit\n");
        read(operation);
        if (x >= 1 and a <= 4) {
            int a;
            int b;
            write("Enter first numer: ");
            read(a);
            write("Enter second number: ");
            read(b);

            write("result: ");
            if (operation == 1) {
                write(a + b);
            } elif (operation == 2) {
                write(a - b);
            } elif (operation == 3) {
                write(a * b);
            } else {
                write(a / b);
            }
            write("\n");
        } else {
            exit();
        }
    }
}
```

## Factorial
```clike
func factorial(int n) {
    if (n <= 1) return n;
    return n*factorial(n-1);
}

func main() {
    int n;
    write("a number to calculate factorial of: ");
    read(n);

    write("the result is: ", factorial(n));
}
```