# List
* [calculator](#calculator) - calculator that have all basic math operations
* [factorial](#factorial) - calculate product of all numbers from 1 to `n`


## Calculator
```clike
func main() {
    loop (true) {
        int operation;
        write("\n", "1. Addition\n", "2. Subtraction\n", 
        "3. Multiplication\n", "4. Division\n", "5.Exit\n");
        read(operation);
        if (operation >= 1 and operation <= 4) {
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
    if (n <= 1) { return(n); }
    return(n*factorial(n-1));
}

func main() {
    int n;
    write("a number to calculate factorial of: ");
    read(n);

    write("the result is: ", factorial(n));
}
```

## BubleSort
```clike
func main() {
    int[] arr = [9, 2, 4, 0, 1, 5, 7, 8];
    int n = arr.size();

    #array before sorting
    loop(int i=0; i<n; i++) {
        write(arr[i], " ");
    }
    write("\n");

    loop(int i=0; i<n-1; i++) {
        loop(int j=0; j<n-i-1; j++) {
            if(arr[j] > arr[j+1]) {
                int a = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = a;
            }
        }
    }
    
    #array after sorting
    loop(int i=0; i<n; i++) {
        write(arr[i], " ");
    }
    write("\n");
    
}
```