# List (string)
* [`toLower`](#toLower) - returns string in lowercase
* [`toUpper`](#toUpper) - returns string in uppercase
* [`charAt`](#charAt) - returns char on passed index
* [`replace`](#replace) - returns string with replaced part
* [`substr`](#substr) - returns substring
* [`split`](#split) - returns an array of strings broken down by key

!> Use `#` instad of `//` for comments

## toLower
This function takes string and returns it in lowercase
```clike
toLower("HeLlo"); // this returns hello
```

## toUpper
This function takes string and returns it in uppercase
```clike
toUpper("HeLlo"); // this returns HELLO
```

## charAt
This function takes string and index nad returns char on that index
```clike
charAt("HeLlo", 3); // this returns l
```

## replace
This function takes string in which you want to do replace, from one value to another
```clike
replace("I love c++", "++", "$"); // this returns "I love c$"
```

## substr
This function takes string, start position for substring and end position for substring
```clike
substr("I said Hello World!", 7, 18); // this returns "Hello World!"
```

## split
This function splits a string according to the given key
```clike
split("What is the best programming language?", " "); // this returns ["What", "is", "the", "best", "programming", "language?"]
```