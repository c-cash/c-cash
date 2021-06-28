# List (math)
* [`round`](#round) - round a number
* [`floor`](#floor) - floor a number
* [`ceil`](#ceil) - ceil a number
* [`pow`](#pow) - get `n`-th power of the number
* [`sqrt`](#sqrt) - get square root of the number
* [`nthroot`](#nthroot) - get `n`-tn root of the number
* [`cos`](#cos) - get cosine of the angle
* [`sin`](#sin) - get sine of the angle
* [`tan`](#tan) - get tangent of the angle
* [`acos`](#cos) - get arcus cosine of the angle
* [`asin`](#sin) - get arcus sine of the angle
* [`atan`](#tan) - get arcus tangent of the angle
* [`atant`](#atant) - get atan2 of two numbers
* [`log`](#log) - get log2 of a number
* [`abs`](#abs) - get absolute value of a number
* [`deg`](#deg) - transform radians to degrees
* [`rad`](#rad) - transform degrees to radians
* [`max`](#max) - transform radians to degrees
* [`min`](#min) - transform degrees to radians
* [`PI`](#PI) - returns PI number
* [`E`](#E) - returns Euler's number

!> Use `#` instad of `//` for comments

## round
This function takes one number and returns a rounded number
```clike
round(5.8); // this is equal to 6 
round(5.4); // this is equal to 5 
```

## floor
This function takes one number and returns a floored number
```clike
floor(5.5); // this is equal to 5 
```

## ceil
This function takes one number and returns a ceiled number
```clike
ceil(5.5); // this is equal to 6 
```

## pow
This function takes two arguments (`a` - number to get the power of; `n` - which power) and returns it's power
```clike
pow(10, 3); // this is equal to 100
```

## sqrt
This function takes one number and returns it's square root
```clike
sqrt(25); // this is equal to 5
```

## nthroot
This function takes two arguments (`a` - number to get the root of; `n` - which root) and returns it's sqare root
```clike
nthroot(27, 3); // this is equal to 3
```

## cos
This function takes angle in radians and returns cosine
```clike
cos(1) // this is equal to 0.5403...
```

## sin
This function takes angle in radians and returns sine
```clike
sin(1) // this is equal to 0.8414... 
```

## tan
This function takes angle in radians and returns tangent
```clike
tan(1) // this is equal to 1.5574...
```

## acos
This function returns the principal value of the arc cosine of x, expressed in radians.
```clike
acos(0.99) // this is equal to 0.1415... rad
```

## asin
This function returns the principal value of the arc sine of x, expressed in radians.
```clike
asin(0.99) // this is equal to 1.4292... rad
```

## atan
This function returns the principal value of the arc tangent of x, expressed in radians.
```clike
atan(0.99) // this is equal to 0.7803... rad
```

## atant
This function takes two numbers (`y`, `x`) and returns their atan2
```clike
atant(1, 1) // this is equal to 0.7853...
```

## log
This function takes a number and returns it's log2
```clike
log(10) // this is equal to 1
```

## abs
This function takes one number and return abs of this number
```clike
abs(-1) // this is equal 1
abs(1)  // this is equal 1
```

## deg
This function takes angle in radians and returns angle in degrees
```clike
deg(57.2957) // this is equal to about 57.2 degree 
```

## rad
This function takes angle in degrees and returns angle in radians
```clike
deg(1) // this is equal to about 0.0174 radians 
```

## max
This function takes one or more numbers and returns greatest of them.
```clike
max(3, 8) // this is equal to 8
```

## min
This function takes one or more numbers and returns smallest of them.
```clike
deg(3, 8) // this is equal to 3
```

## PI
This function doesn't takes arguments and returns PI number
```clike
PI() // this is equal to 3.141592
```

## E
This function doesn't takes arguments and returns Euler's number
```clike
E() // this is equal to 2.718281
```