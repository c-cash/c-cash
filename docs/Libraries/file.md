# List (file)
* [`loadFile`](#loadFile) - reads the contents of the file
* [`saveFile`](#saveFile) - saves a file with the given text
* [`appendFile`](#appendFile) - adds text to the file
* [`clearFile`](#clearFile) - clears the file
* [`delFile`](#delFile) - deletes the file

!> Use `#` instad of `//` for comments

## loadFile
This function takes path to file and returns its contents
```clike
loadFile("C:\Users\Cash\cash.txt"); // this returns content of cash.txt file
```

## saveFile
This function takes path to file and overwrite it with given string
```clike
saveFile("C:\Users\Cash\cash.txt", "C$ is great"); // this overwrite cash.txt file with "C$ is great"
```

## appendFile
This function takes path to file and write to it given string
```clike
appendFile("C:\Users\Cash\cash.txt", "C$ is great"); // this write to cash.txt file "C$ is great"
```

## clearFile
This function takes path to file and clears it
```clike
clearFile("C:\Users\Cash\cash.txt"); // this clears the content of cash.txt file
```

## delFile
This function takes path to file and deletes it
```clike
delFile("C:\Users\Cash\cash.txt"); // this deletes cash.txt file
```