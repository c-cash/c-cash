# Syntax
## Functions
In C$ you can define functions. Function is a piece of code that can be reused within different places of you program.
Defining functions in C$ is super easy! All you need to do is write code like this below
```clike
func main() {
}
```
!> You code must have a `main` function to work
### Arguments
Functions can also take and return data. For example function `add` from below will take two numbers (a and b) and return their sum.
```clike
func add(int a, int b) {
    return(a+b);
}
func main() {
    write(add(1, 2));
} 
```
## Variables
### Types
Variables can hold only specified type of data:
- `int` - simple number
- `uint` - unsigned integer
- `char` - one character
- `uchar` - unsigned character
- `double` - decimal number
- `string` - text
- `bool` - true/false

### Declaration
You can also define and use variables. To define a variable, write its type followed by name (like below).
```clike
func main() {
    int a;
    int b, c;
}
```
To declare the table you must do this
```clike
func main(){
    int[] a = [2, 4, 2];
    string[] b = ["Hello", "World!"];
}
```
### Assignment
To assign value to a variable write it's name followed by equal sign and a new value.
```clike
func main() {
    int a;
    a = 4;
}
```
Or assign it directly after creation
```clike
func main() {
    int a = 4;
    int b = 2, c = 3;
}
```
You can also use the digit separator if you like.
```clike
func main() {
    int a = 1_000_000;
}
```
## Math
You can use math expressions in many places. Currently supported operators are:
- `+` - add
- `-` - subtract
- `/` - divide
- `*` - multiply
- `%` - rest of the division


Assuming that `a` is a variable, there are also:
- `a++` - add 1 to `a`
- `a--` - subtract 1 from `a`
- `a+=4` - add value on the right to `a`
- `a-=4` - subtract value on the right from `a`
- `a/=4` - divide `a` by value on the right
- `a*=4` - multiply `a` by value on the right
- `a%=4` - rest of the division from `a` by value on the right

## Array
The function you can use on array are:
- `add()` - add varibles to array
- `size()` - return number of elements

Example code:
```clike
func main(){
    int[] a = [3, 4, 5];
    a.add(2, 4, 5);
    write(a.size());
    write(a[0]);
}
```

## Statements
### Expressions
For `if` and `loop` statements you will need expressions. Currently supported are:
- `==` - if things are equal
- `!=` - if things are NOT equal
- `<` - less than
- `<=` - less or equal to
- `>` - greater
- `>=` - greater or equal to
- `and` - only if both expression (to the left and to the right) return `true`
- `or` - if either one or both expressions return `true`
### if/else/elif
You can use if to execute code only when expression returns `true`.
Otherwise, if expression returns `false` then else will be executed.
For example:
```clike
func main() {
    if (1 == 1) {
        write("Hello World");
    } else {
        write("World Hello");
    }
}
```
To decrease code size, there is also an elif statement, that works like `else` but will be executed only when expression returns `true`
Example:
```clike
func main() {
    if (1 == 2) {
        write("This won't be executed");
    } elif (1 == 1) {
        write("This will be executed :D");
    } else {
        write("This won't be executed too");
    }
}
```
### loop
In many languages there are two types of loop: `for` and `while`. In C$ we have only one, that does three things!
You can use `loop` with expression inside (like `while`) or with three arguments (like `for`) or provide it just a number to execute code `n` times.
```clike
// while loop
int a=0;
loop(a < 5) {
    write(a);
    a++;
}
// for loop
loop(int i=0, i<5, i++) {
    write(i);
}
// n loop
loop(5) {
    write("n loop");
}
```

!> Two slashes in comments are not necessary for comment and they are only because of syntax highlighting