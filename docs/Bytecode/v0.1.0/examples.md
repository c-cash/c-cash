```clike
func main() {
    int a = 5;
    int b = a + 2;
    write(b);
}
```
translates into bytecode as
```js
$Main:
    static main 0:
        iconst 5
        istore 0

        iload 0
        iconst 2
        iadd
        istore 1

        iload 1
        callstatic "$Builtin" "write"
```
---
```clike
func add(int a, int b) {
    return(a+b);
}

func main() {
    int a = add(2, 3);
}
```
translates into bytecode as
```js
$Main:
    static add 2:
        iadd
        ireturn
    
    static main 0:
        iconst 2
        iconst 3
        callstatic "$Main" "add"
        istore
```