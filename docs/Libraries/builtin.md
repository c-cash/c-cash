# List
* [`write`](#write) - write something to the console
* [`read`](#read) - read input from console
* [`exit`](#exit) - stop the program
* [`typeof`](#typeof) - return the type of a given variable
* [`system`](#system) - enables system management

## write
`write` function allows you to write anything to the console.
You can provide one or more arguments. Example below will write `"Hello World"`
```clike
func main() {
    write("Hello", "World");
}
```

## read
`read` function takes one variable as an argument. It will wait for user input and then set variable value to whatever user typed in.
Example below will ask user for teir age, and then use it in a message
```clike
func main() {
    int a;
    write("How old are you?\n");
    read(a);
    write("You are ", a, " years old!");
}
```

## exit
`exit` function exits from the program. For example code below will print numbers only from 1 to 5
```clike
func main() {
    for (int i=0, i<10, i++) {
        write(i);
        if (i == 5) {
            exit();
        }
    }
}
```

## typeof
`typeof` function returns the type of a given variable
```clike
func main() {
    int a;
    write(typeof(a));
}
```

## system
`system` function enables system management
```clike
func main() {
    int a;
    system("echo c-cash");
}
```