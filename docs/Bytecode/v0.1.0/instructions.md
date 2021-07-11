# types

| prefix | C$ Type   | Description                            | Accepted values                     |
| ------ | --------- | -------------------------------------- | ----------------------------------- |
| `i`    | `int`     | 32 bit signed number                   | -2<sup>31</sup> → 2<sup>31</sup>-1  |
| `l`    | `long`    | 64 bit signed number                   | -2<sup>63</sup> → 2<sup>63</sup>-1  |
| `d`    | `double`  | 64 bit signed floating point number    | N/A                                 |
| `c`    | `char`    | ?? bit unsigned number                 | 0 → 65 535                          |
| `b`    | `boolean` | 1 bit value                            | 0 → 1                               |
| `o`    | `Object`  | Reference to the object in memory heap | N/A                                 |

---
# instructions

| Mnemonic | hex opcode | opcode    | data bytes                    | stack before -> after    | Description                            |
| -------- | ---------- | --------- | ------------------------------| ---------------------    | -------------------------------------- |
| iconst   | 00         | 0000 0000 | value                         | → [value]                | pushes integer value onto the stack |
| istore   | 01         | 0000 0001 | index                         | value →                  | stores int value into variable #index |
| iastore  | 02         | 0000 0010 |                               | arrayref, index, value → | stores int value into an array |
| iload    | 03         | 0000 0011 | index                         | → value                  | loads value from the memory #index |
| iaload   | 04         | 0000 0100 |                               | arrayref, index → value  | loads value from the array |
| iadd     | 05         | 0000 0101 |                               | value1, value2 → result | adds two integers |
| isub     | 06         | 0000 0110 |                               | value1, value2 → result | subtracts two integers |
| imul     | 07         | 0000 0111 |                               | value1, value2 → result | multiplies two integers |
| idiv     | 08         | 0000 1000 |                               | value1, value2 → result | divides two integers |
| lconst   | 09         | 0000 1001 | value1, value2                | → [value1 < 32 | value2]  | pushes long value onto the stack |
| lstore   | 0A         | 0000 1010 | index                         | value →                  | stores long value into variable #index |
| lastore  | 0B         | 0000 1011 |                               | arrayref, index, value → | stores long value into an array |
| lload    | 0C         | 0000 1100 | index                         | → value                  | loads value from the memory #index |
| laload   | 0D         | 0000 1101 |                               | arrayref, index → value  | loads value from the array |
| ladd     | 0E         | 0000 1110 |                               | value1, value2 → result | adds two longs |
| lsub     | 0F         | 0000 1111 |                               | value1, value2 → result | subtracts two longs |
| lmul     | 10         | 0001 0000 |                               | value1, value2 → result | multiplies two longs |
| ldiv     | 11         | 0001 0001 |                               | value1, value2 → result | divides two longs |
| dconst   | 12         | 0001 0010 | value1, value2                | → [value1 < 32 | value2] | pushes double value onto the stack |
| dstore   | 13         | 0001 0011 | index                         | value →                  | stores double value into variable #index |
| dastore  | 14         | 0001 0100 |                               | arrayref, index, value → | stores double value into an array |
| dload    | 15         | 0001 0101 | index                         | → value                  | loads value from the memory #index |
| daload   | 16         | 0001 0110 |                               | arrayref, index → value  | loads value from the array |
| dadd     | 17         | 0001 0111 |                               | value1, value2 → result | adds two doubles |
| dsub     | 18         | 0001 1000 |                               | value1, value2 → result | subtracts two doubles |
| dmul     | 19         | 0001 1001 |                               | value1, value2 → result | multiplies two doubles |
| ddiv     | 1A         | 0001 1010 |                               | value1, value2 → result | divides two doubles |
| cconst   | 1B         | 0001 1011 | value                         | → [0b11111111 & value]   | pushes char value onto the stack |
| cstore   | 1C         | 0001 1100 | index                         | value →                  | stores char value into variable #index |
| castore  | 1D         | 0001 1101 |                               | arrayref, index, value → | stores char value into an array |
| cload    | 1E         | 0001 1110 | index                         | → value                  | loads value from the memory #index |
| caload   | 1F         | 0001 1111 |                               | arrayref, index → value  | loads value from the array |
| cadd     | 20         | 0010 0001 |                               | value1, value2 → result | adds two chars |
| csub     | 21         | 0010 0010 |                               | value1, value2 → result | subtracts two chars |
| bconst   | 22         | 0010 0011 | value                         | → [0b1 & value]          | pushes boolean value onto the stack |
| bstore   | 23         | 0010 0100 | index                         | value →                  | stores boolean value into variable #index |
| bastore  | 24         | 0010 0101 |                               | arrayref, index, value → | stores boolean value into an array |
| bload    | 25         | 0010 0110 | index                         | → value                  | loads value from the memory #index |
| baload   | 26         | 0010 0111 |                               | arrayref, index → value  | loads value from the array |
| onull    | 27         | 0010 1000 |                               | → null                   | pushes null object onto the stack |
| oconst   | 28         | 0010 1001 | UTF8                          | [arg1, arg2, ...] → objectref | instantiates new instance of a class [UTF8] |
| callstatic | 29       | 0010 1010 | UTF8, UTF8                    | [arg1, arg2, ...] → result | calls static function [UTF8] from the class [UTF8] |
| callmethod | 2A       | 0010 1011 | UTF8                          | objectref, [arg1, arg2, ...] → result | calls method [UTF8] from the objectref |
| ovload   | 2B         | 0010 1100 | UTF8                          | objectref → value      | gets value from field [UTF8] in the objectref |
| ovstore  | 2C         | 0010 1101 | UTF8                          | objectref, value →     | stores value in field [UTF8] of the objectref |
| ostore   | 2D         | 0010 1110 | index                         | objectref →            | stores objectref into variable #index |
| oastore  | 2E         | 0010 1111 |                               | arrayref, index, objectref → | stores objectref into an array |
| oload    | 2F         | 0011 0000 | index                         | → objectref            | loads objectref from variable #index |
| oaload   | 30         | 0011 0001 |                               | arrayref, index → objectref | loads objectref from the array |
| goto     | 31         | 0011 0010 | index1, index2                | [no change]             | jumps to the instruction at [index1 < 32 \| index2] |
| ifeq     | 32         | 0011 0011 | index1, index2                | value1, value2 →        | if values are equal then jump to the instruction at [index1 < 32 \| index2] |
| ifge     | 33         | 0011 0100 | index1, index2                | value1, value2 →        | if value1 is greater or equal to value2 then jump to the instruction at [index1 < 32 \| index2] |
| ifgt     | 34         | 0011 0101 | index1, index2                | value1, value2 →        | if value1 is greater than value2 then jump to the instruction at [index1 < 32 \| index2] |
| ifne     | 35         | 0011 0110 | index1, index2                | value1, value2 →        | if value1 is not equal to value2 then jump to the instruction at [index1 < 32 \| index2] |
| ifnull    | 36         | 0011 0111 | index1, index2                | objectref →        | if objectref is null then jump to the instruction at [index1 < 32 \| index2] |
| ifnotnull | 37         | 0011 1000 | index1, index2                | objectref →        | if objectref is not null then jump to the instruction at [index1 < 32 \| index2] |
| instanceof | 38         | 0011 1001 | UTF8                         | objectref → boolean | returns true if objectref is an instance of [UTF8] |
| throw     | 39         | 0011 1010 |                               | objectref →         | throws an error from the error objectref |
| return | 3A            | 0011 1011 |                               | → [empty]           | returns void from a method |
| ireturn | 3B            | 0011 1100 |                               | value → [empty]           | returns int from a method |
| lreturn | 3C            | 0011 1101 |                               | value → [empty]           | returns long from a method |
| dreturn | 3D            | 0011 1110 |                               | value → [empty]           | returns double from a method |
| creturn | 3E            | 0011 1111 |                               | value → [empty]           | returns char from a method |
| breturn | 3F            | 0100 0000 |                               | value → [empty]           | returns boolean from a method |
| oreturn | 40            | 0100 0001 |                               | objectref → [empty]       | returns objectref from a method |
| i2l     | 41            | 0100 0010 |                               | value → result            | convert an int to a long |
| i2d     | 42            | 0100 0011 |                               | value → result            | convert an int to a double |
| i2c     | 43            | 0100 0100 |                               | value → result            | convert an int to a char |
| i2b     | 44            | 0100 0101 |                               | value → result            | convert an int to a boolean |
| d2i     | 45            | 0100 0110 |                               | value → result            | convert a double to an int |
| d2l     | 46            | 0100 0111 |                               | value → result            | convert a double to a long |
| d2b     | 47            | 0100 1000 |                               | value → result            | convert a double to a boolean |
| l2i     | 48            | 0100 1001 |                               | value → result            | convert a long to an integer |
| l2d     | 49            | 0100 1010 |                               | value → result            | convert a long to a double |
| l2c     | 4A            | 0100 1011 |                               | value → result            | convert a long to a char |
| l2b     | 4B            | 0100 1100 |                               | value → result            | convert a long to a boolean |
| c2i     | 4C            | 0100 1101 |                               | value → result            | convert a char to an int |
| c2l     | 4D            | 0100 1110 |                               | value → result            | convert a char to a long |
| arrconst  | 4E          | 0100 1111 |                               | → arrayref                | create a new empty array |
| arrstore  | 4F          | 0101 0000 | index                         | value →                  | stores arrayref into variable #index |
| arrastore | 50          | 0101 0001 |                               | arrayref, index, value → | stores arrayref into an array |
| arrload   | 51          | 0101 0010 | index                         | → value                  | loads arrayref from the memory #index |
| arraload  | 52          | 0101 0011 |                               | arrayref, index → value  | loads arrayref from the array |