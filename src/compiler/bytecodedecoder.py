
# 00100100 01001101 01100001 01101001 01101110 11111111 00000010 00000000 00000000 00000000 00000001 00000000 00000000 00000000 00101010 00000000 00000000 00000000 00000000 11111111 00000100 00000000 00000000 00000000 01101101 01100001 01101001 01101110 00000000 00000000 00000000 00000000 00000000 00000010 00000000 00000000 00000000 00000001 00000000 00000000 00000000 00000000 11111111
# $        M        a        i        n        NOP      <---------------int---------------> <---------------int---------------> *        <---------------int---------------> NOP
# settings
filepath = r"/home/antis/ccash/ccash-main/build/test.ccbin"

def loadInt(f):
    temp = f.read(4)
    i = int.from_bytes(temp, byteorder='little', signed=True)
    return str(i)

def loadLong(f):
    temp = f.read(8)
    i = int.from_bytes(temp, byteorder='little', signed=True)
    return str(i)
def loadDouble(f):
    temp = f.read(8)
    t = int.from_bytes(temp, byteorder='little', signed=True)
    i = float.fromhex(hex(t))
    return str(t)
def loadByte(f):
    return "{:08b}".format(int(f.read(1).hex(),16))[7]
def loadUTF8(f):
    r = ""
    l = int(loadInt(f))
    for _ in range(l):
        r += chr(int(f.read(1).hex(), 16))
    return r

def methodFlags(flags):
    f = "{:08b}".format(int(flags.hex(),16))
    print("\t", end='')
    if f[7] == "1":
        print("static", end=' ')
    if f[6] == "1":
        print("private", end=' ')

# code
def main():
    NOP = "11111111"
    ci = 0
    with open(filepath, "rb") as f:
        cname = ""
        while byte := f.read(1):
            b = "{:08b}".format(int(byte.hex(),16))
            if b == NOP:
                print(cname + ":")
                cname = ""
                l = int(loadInt(f))
                # load function name
                for _ in range(l):
                    fname = ""
                    flags = f.read(1)
                    methodFlags(flags)
                    flen = int(loadInt(f))
                    for _ in range(flen):
                        fname += chr(int(f.read(1).hex(), 16))
                    print(fname + " " + loadInt(f) + ":")
                    # load instructions
                    ci = -1
                    while t := f.read(1):
                        ins = parse(t, f)
                        ci+=1
                        if ins == "NOP":
                            break
                        print("\t\t" + str(ci) + ": " + ins)

            else:
                cname += chr(int(byte.hex(), 16))
            


        # while byte := f.read(1):
        #     print("{:08b}".format(int(byte.hex(),16)), end=" ")
        #     code += ("\t" * ident) + parse(byte, f) + "\n"

def parse(code, f):
    opcode = "{:08b}".format(int(code.hex(),16))

    if opcode == "00000000":
        return "iconst " + loadInt(f)
    elif opcode == "00000001":
        return "istore " + loadInt(f)
    elif opcode == "00000010":
        return "iastore"
    elif opcode == "00000011":
        return "iload " + loadInt(f)
    elif opcode == "00000100":
        return "iaload"
    elif opcode == "00000101":
        return "iadd"
    elif opcode == "00000110":
        return "isub"
    elif opcode == "00000111":
        return "imul"
    elif opcode == "00001000":
        return "idiv"

    if opcode == "00001001":
        return "lconst " + loadLong(f)
    elif opcode == "00001010":
        return "lstore " + loadInt(f)
    elif opcode == "00001011":
        return "lastore"
    elif opcode == "00001100":
        return "lload " + loadInt(f)
    elif opcode == "00001101":
        return "laload"
    elif opcode == "00001110":
        return "ladd"
    elif opcode == "00001111":
        return "lsub"
    elif opcode == "00010000":
        return "lmul"
    elif opcode == "00010001":
        return "ldiv"

    if opcode == "00010010":
        return "dconst " + loadDouble(f)
    elif opcode == "00010011":
        return "dstore " + loadInt(f)
    elif opcode == "00010100":
        return "dastore"
    elif opcode == "00010101":
        return "dload " + loadInt(f)
    elif opcode == "00010110":
        return "daload"
    elif opcode == "00010111":
        return "dadd"
    elif opcode == "00011000":
        return "dsub"
    elif opcode == "00011001":
        return "dmul"
    elif opcode == "00011010":
        return "ddiv"

    if opcode == "00011011":
        return "ccconst " + loadByte(f)
    elif opcode == "00011100":
        return "cstore " + loadInt(f)
    elif opcode == "00011101":
        return "castore"
    elif opcode == "00011110":
        return "cload " + loadInt(f)
    elif opcode == "00011111":
        return "caload"
    elif opcode == "00100000":
        return "cadd"
    elif opcode == "00100001":
        return "csub"

    if opcode == "00100010":
        return "bconst " + loadByte(f)
    elif opcode == "00100011":
        return "bstore " + loadInt(f)
    elif opcode == "00100100":
        return "bastore"
    elif opcode == "00100101":
        return "bload " + loadInt(f)
    elif opcode == "00100110":
        return "baload"

    elif opcode == "00101101":
        return "ostore " + loadInt(f)
    elif opcode == "00101110":
        return "oastore"
    elif opcode == "00101111":
        return "oload " + loadInt(f)
    elif opcode == "00110000":
        return "oaload"

    elif opcode == "00110010":
        return "ifeq"
    elif opcode == "00110011":
        return "ifge "
    elif opcode == "00110100":
        return "ifgt"
    elif opcode == "00110101":
        return "ifne"
    elif opcode == "00110001":
        return "goto " + loadLong(f)
    elif opcode == "01010101":
        return "not "

    elif opcode == "00101001":
        return "callstatic " + loadUTF8(f) + " " + loadUTF8(f)
    elif opcode == "00101010":
        return "callmethod " + loadUTF8(f)

    elif opcode == "01010011":
        return "jmptrue " + loadLong(f)
    elif opcode == "01010100":
        return "jmpfalse " + loadLong(f)

    elif opcode == "00111010":
        return "return"
    elif opcode == "00111011":
        return "ireturn"
    elif opcode == "00111100":
        return "lreturn"
    elif opcode == "00111101":
        return "deturn"
    elif opcode == "00111110":
        return "creturn"
    elif opcode == "00111111":
        return "breturn"

    elif opcode == "01000001":
        return "i2l"
    elif opcode == "01000010":
        return "i2d"

    elif opcode == "01001000":
        return "l2i"

    elif opcode == "01011001":
        return "anystore " + loadInt(f)

    elif opcode == "00101000":
        return "oconst " + loadUTF8(f)

    elif opcode == "11111101":
        return "NULL"
    elif opcode == "11111111":
        return "NOP"

    return "unknown: " + str(opcode)
    
main()