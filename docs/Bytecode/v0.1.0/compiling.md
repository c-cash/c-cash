# Saving
In c$ programs are compiled into simple bytecode, it's instructions are available on [`INSTRUCTIONS`](Bytecode/v0.1.0/instructions.md) page.\
Each global method is compiled as it is a part of the `$Main` class.\
For example function below:
```clike
func main() {
    int a = 5;
}
```
Will be compiled as shown below:
```js
$Main:
    static main 0:
        iconst 5
        istore 0
```
Where $Main is a name of the class, and `0` after `main` is a number of arguments that this function takes.

---
In case of many functions it will be compiled as shown below:
```clike
#// this function starts with 2 elements in the memory heap
func add(int a, int b) {
    return(a + b);
}
func main() {
    int a = add(2, 3);
}
```
```js
$Main:
    static add 2:
        iload 0
        iload 1
        iadd
        ireturn
    static main 0:
        iconst 2
        iconst 3
        callstatic "$Main" "add"
        istore 0
```