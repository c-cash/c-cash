# List (string)
* [`toLower`](#toLower) - returns string in lowercase
* [`toUpper`](#toUpper) - returns string in uppercase
* [`charAt`](#charAt) - returns char on passed index
* [`replace`](#replace) - returns string with replaced part
* [`substr`](#substr) - returns substring
* [`split`](#split) - returns an array of strings broken down by key
* [`reverse`](#reverse) - returns reversed string
* [`capitalize`](#capitalize) - returns capitalized string
* [`swapCase`](#swapCase) - returns string with swaped case
* [`startsWith`](#startsWith) - returns if string starts with the given phrase 
* [`endsWith`](#endsWith) - returns if string ends with the given phrase 
* [`find`](#find) - returns first index of searched string and if it doesn't exist returns -1
* [`delWhitespace`](#delWhitespace) - returns string with removed all whitespaces
* [`strip`](#strip) - returns string with removed all whitespaces from beginning and end and with removed double spaces
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

## reverse
This function reverses a string
```clike
reverse("murder"); // this returns "redrum"
```

## capitalize
This function capitalizes a string
```clike
capitalize("info"); // this returns "Info"
```

## swapCase
This function swaps case in each letter
```clike
swapCase("AnTiS"); // this returns "aNtIs"
```

## startsWith
This function returns if string starts with the given phrase 
```clike
startsWith("Antis", "Ant"); // this returns true
startsWith("Antis", "Abcd"); // this returns false
```

## endsWith
This function returns if string ends with the given phrase 
```clike
endsWith("Antis", "tis"); // this returns true
endsWith("Antis", "xyz"); // this returns false
```

## find
This function takes two arguments and returns first index of searched string and if it doesn't exist returns -1
```clike
find("info", "nf"); // this returns 1
find("info", "xyz"); // this returns -1
```

## delWhitespace
This function takes one argument and removes all whitespaces from string
```clike
delWhitespace(" in f o  "); // this returns "info"
```

## strip
This function takes one argument and returns string with removed all whitespaces from beginning and end and with removed double spaces
```clike
strip(" info  is    great  "); // this returns "info is great"
```