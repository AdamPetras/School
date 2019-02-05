#!/usr/bin/env python3
import sys
globalSymbolTable = {}
localSymbolTable = []
temporarySymbolTable = {}
labelTable = {}
programCounter = 0
instrDone  = 0
callStack = []
dataStack = []
frameCreated = False
class Instruction:
    def __init__(self,instr,numb,arg1,arg2,arg3):
        self.instr = instr
        self.numb = numb
        if arg1:
            if "var" in arg1[0]:
                arg1.append(str(arg1[1].split('@')[0]))
                arg1[1]=arg1[1].split('@')[1]
        self.arg1 = arg1
        if arg2:
            if "var" in arg2[0]:
                arg2.append(str(arg2[1].split('@')[0]))
                arg2[1]=arg2[1].split('@')[1]
        self.arg2 = arg2
        if arg3:
            if "var" in arg3[0]:
                arg3.append(str(arg3[1].split('@')[0]))
                arg3[1]=arg3[1].split('@')[1]
        self.arg3 = arg3

    def writeInstruction(self):
        print((self.instr))

def SwitchingSemantic(instArg, pattArg,instr):
    if pattArg == "var":
        if instr.instr != "DEFVAR":
            if instArg[2] == "GF":
                if not any(instArg[1] in x for x in globalSymbolTable):
                        Debug("temporary is not in global symbol table")
            if instArg[2] == "LF":
                if not any(instArg[1] in x for x in localSymbolTable):
                        Debug("temporary is not in local symbol table")
        if instArg[0] != "var":
            Debug("argument is not var")
            return False
    elif pattArg == "symbol":
        if instArg[0] == "label":
            Debug("argument is not symbol")
            return False
    elif pattArg == "label":
        if instArg[0] != "label":
            Debug("argument is not label")
            return False
    elif pattArg == "type":
        if instArg[0] != "string" and instArg[0] != "int" and instArg[0] != "bool":
            Debug("argument is not type")
            return False
    elif pattArg == "empty":
        if instArg:
            Debug("argument is not empty")
            return False
    return True

def SemanticControl(instruction, patternInst):
    if not instruction:
        return False
    if not patternInst:
        return False
    if not patternInst.instr in instruction.instr:
        return False
    if not SwitchingSemantic(instruction.arg1,patternInst.arg1,instruction):
        return False
    if not SwitchingSemantic(instruction.arg2,patternInst.arg2,instruction):
        return False
    if not SwitchingSemantic(instruction.arg3,patternInst.arg3,instruction):
        return False
    return True

def Debug(string):
    print(string)

def SelectInstrFromList(lst, objname):
    for x in lst:
        if x.instr == objname:
            return x
    return None

def SetValueInTable(arg,name,value,mytype):
    global globalSymbolTable
    global localSymbolTable
    global temporarySymbolTable
    if arg[2] == "GF":
        for x in globalSymbolTable:
            if x == name:
                globalSymbolTable[x] = [value,mytype]
    if arg[2] == "LF":
        for x in localSymbolTable[0]:
            if x == name:
                localSymbolTable[0][x] = [value,mytype]
    if arg[2] == "TF":
        for x in temporarySymbolTable:
            if x == name:
                temporarySymbolTable[x] = [value,mytype]
    

def GetValueFromTable(lst,name,value):
    for x in lst:
        return x[name]

def SelectTemporaryFromTable(arg):
    if arg[0] == "var":
        global globalSymbolTable
        global localSymbolTable
        global temporarySymbolTable
        if arg[2] == "GF":
            for x in globalSymbolTable:
                return x
            sys.stderr.write("Variable is not in global frame\n")
            sys.exit(54)
        if arg[2] == "LF":
            for x in localSymbolTable[0]:
                return x
            sys.stderr.write("Variable is not in local frame\n")
            sys.exit(54)
        if arg[2] == "TF":
            for x in temporarySymbolTable:
                return x
            sys.stderr.write("Variable is not in temporary frame\n")
            sys.exit(54)

    else:
        return arg[1]
def SelectVariableFromTable(arg):
    if arg[0] == "var":
        global globalSymbolTable
        global localSymbolTable
        global temporarySymbolTable
        if arg[2] == "GF":
            try:
                return globalSymbolTable[arg[1]][0]
            except KeyError:
                sys.stderr.write("Variable is not in global frame\n")
                sys.exit(54)
                
        if arg[2] == "LF":
            try:
                return localSymbolTable[0][arg[1]][0]
            except KeyError:
                sys.stderr.write("Variable is not in local frame\n")
                sys.exit(54)
            except IndexError:
                sys.stderr.write("Local frame doesnt exist\n")
                sys.exit(55)
        if arg[2] == "TF":
            try:
                return temporarySymbolTable[arg[1]][0]
            except KeyError:
                sys.stderr.write("Variable is not in temporary frame\n")
                sys.exit(54)
            except IndexError:
                sys.stderr.write("Temporary frame doesnt exist\n")
                sys.exit(55)
    return arg[1]

def SelectTypeFromTable(arg):
    if arg[0] == "var":
        global globalSymbolTable
        global localSymbolTable
        global temporarySymbolTable
        if arg[2] == "GF":
            try:
                return globalSymbolTable[arg[1]][1]
            except KeyError:
                sys.stderr.write("Variable is not in global frame\n")
                sys.exit(54)
        if arg[2] == "LF":
            try:
                return localSymbolTable[0][arg[1]][1]
            except KeyError:
                sys.stderr.write("Variable is not in local frame\n")
                sys.exit(54)
            except IndexError:
                sys.stderr.write("Local frame doesnt exist\n")
                sys.exit(55)
        if arg[2] == "TF":
            try:
                return temporarySymbolTable[arg[1]][1]
            except KeyError:
                sys.stderr.write("Variable is not in temporary frame\n")
                sys.exit(54)
            except IndexError:
                sys.stderr.write("Temporary frame doesnt exist\n")
                sys.exit(55)
    return ReturnType(arg[1])

def isSybol(typearg):
    if typearg == "int":
        return True
    if typearg == "string":
        return True
    if typearg == "bool":
        return True
    if typearg == "var":
        return True
    return False

def ReturnType(argValue):
    if is_number(argValue):
        return "int"
    elif argValue == "true" or argValue == "false":
        return "bool"
    else:
        return "string"

def ConvertString(string):
    index = str(string).find('\\')
    while index != -1:
        string = string[:index] + chr(int(string[index+1:index+4])) + string[index+4:]    
        if(string[index] == '\\'):
            index+=1
        index = str(string).find('\\', index)
    return string

def Move(arg1,arg2):
    if arg1[0] == "var":
        if isSybol(arg2[0]):
            var = ""
            if arg2[1]:
                var = ConvertString(arg2[1])
            if arg2[0] == "var":
                var = ConvertString(SelectVariableFromTable(arg2))
            SetValueInTable(arg1,arg1[1],var,ReturnType(var))
        else:
            sys.stderr.write("arg2 u move neni symbol\n")
            sys.exit(53)
    else:
        sys.stderr.write("arg1 u move neni variable\n")
        sys.exit(53)

def Add(arg1,arg2,arg3):
    if arg1[0] == "var":
        if isSybol(arg2[0]):
            if isSybol(arg3[0]):
                if arg2[0] == "int" and arg3[0] == "int":
                        var = int(arg2[1])+int(arg3[1])
                        SetValueInTable(arg1,arg1[1],var,ReturnType(var))
                elif arg2[0] == "var" or arg3[0] == "var":
                    var1 = arg2[1]
                    var2 = arg3[1]
                    if arg2[0] == "var":
                        if SelectTypeFromTable(arg2) == "int":
                            var1 = SelectVariableFromTable(arg2)
                        else:
                            sys.stderr.write("arg2 u add neni int\n")
                            sys.exit(53)
                    if arg3[0] == "var": 
                        if SelectTypeFromTable(arg2) == "int":
                            var2 = SelectVariableFromTable(arg3)
                        else:
                            sys.stderr.write("arg3 u add neni int\n")
                            sys.exit(53)
                    var = int(var1)+int(var2)
                    SetValueInTable(arg1,arg1[1],var,ReturnType(var))
                else:
                    sys.stderr.write("arg2 nebo arg3 u add neni int\n")
                    sys.exit(53)
            else:
                sys.stderr.write("arg3 u add neni symbol\n")
                sys.exit(53)
        else:
            sys.stderr.write("arg2 u add neni symbol\n")
            sys.exit(53)
    else:
        sys.stderr.write("arg1 u add neni variable\n")
        sys.exit(53)

def Sub(arg1,arg2,arg3):
    if arg1[0] == "var":
        if isSybol(arg2[0]):
            if isSybol(arg3[0]):
                if arg2[0] == "int" and arg3[0] == "int":
                    var = int(arg2[1])-int(arg3[1])
                    SetValueInTable(arg1,arg1[1],var,ReturnType(var))
                elif arg2[0] == "var" or arg3[0] == "var":
                    var1 = arg2[1]
                    var2 = arg3[1]
                    if arg2[0] == "var":
                        if SelectTypeFromTable(arg2) == "int":
                            var1 = SelectVariableFromTable(arg2)
                        else:
                            sys.stderr.write("arg2 u sub neni int\n")
                            sys.exit(53)
                    if arg3[0] == "var":
                        if SelectTypeFromTable(arg2) == "int":
                            var2 = SelectVariableFromTable(arg3)
                        else:
                            sys.stderr.write("arg3 u sub neni int\n")
                            sys.exit(53)
                    var = int(var1)-int(var2)
                    SetValueInTable(arg1,arg1[1],var,ReturnType(var))
                else:
                    sys.stderr.write("arg2 nebo arg3 u sub neni int\n")
                    sys.exit(53)
            else:
                sys.stderr.write("arg3 u sub neni symbol\n")
                sys.exit(53)
        else:
            sys.stderr.write("arg2 u sub neni symbol\n")
            sys.exit(53)
    else:
        sys.stderr.write("arg1 u sub neni variable\n")
        sys.exit(53)

def Mul(arg1,arg2,arg3):
    if arg1[0] == "var":
        if isSybol(arg2[0]):
            if isSybol(arg3[0]):
                if arg2[0] == "int" and arg3[0] == "int":
                        var = int(arg2[1])*int(arg3[1])
                        SetValueInTable(arg1,arg1[1],var,ReturnType(var))
                elif arg2[0] == "var" or arg3[0] == "var":
                    var1 = arg2[1]
                    var2 = arg3[1]
                    if arg2[0] == "var":
                        if SelectTypeFromTable(arg2) == "int":
                            var1 = SelectVariableFromTable(arg2)
                        else:
                            sys.stderr.write("arg2 u mul neni int\n")
                            sys.exit(53)
                    if arg3[0] == "var":
                        if SelectTypeFromTable(arg2) == "int":
                            var2 = SelectVariableFromTable(arg3)
                        else:
                            sys.stderr.write("arg3 u mul neni int\n")
                            sys.exit(53)
                    var = int(var1)*int(var2)
                    SetValueInTable(arg1,arg1[1],var,ReturnType(var))
                else:
                    sys.stderr.write("arg2 nebo arg3 u mul neni int\n")
                    sys.exit(53)
            else:
                sys.stderr.write("arg3 u mul neni symbol\n")
                sys.exit(53)
        else:
            sys.stderr.write("arg2 u mul neni symbol\n")
            sys.exit(53)
    else:
        sys.stderr.write("arg1 u mul neni variable\n")
        sys.exit(53)

def IDiv(arg1,arg2,arg3):
    if arg1[0] == "var":
        if isSybol(arg2[0]):
            if isSybol(arg3[0]):
                if arg2[0] == "int" and arg3[0] == "int":
                    try:
                        var = int(arg2[1])//int(arg3[1])
                    except ZeroDivisionError:
                        sys.stderr.write("Error Division by zero\n")
                        sys.exit(57)
                    SetValueInTable(arg1,arg1[1],var,ReturnType(var))
                elif arg2[0] == "var" or arg3[0] == "var":
                    var1 = arg2[1]
                    var2 = arg3[1]
                    if arg2[0] == "var":
                        if SelectTypeFromTable(arg2) == "int":
                            var1 = SelectVariableFromTable(arg2)
                        else:
                            sys.stderr.write("arg2 u idiv neni int\n")
                            sys.exit(53)
                    if arg3[0] == "var":
                        if SelectTypeFromTable(arg2) == "int":
                            var2 = SelectVariableFromTable(arg3)
                        else:
                            sys.stderr.write("arg3 u idiv neni int\n")
                            sys.exit(53)
                    try:
                        var = int(var1)//int(var2)
                    except ZeroDivisionError:
                        sys.stderr.write("Error Division by zero\n")
                        sys.exit(57)

                    SetValueInTable(arg1,arg1[1],var,ReturnType(var))
                else:
                    sys.stderr.write("arg2 nebo arg3 u idiv neni int\n")
                    sys.exit(53)
            else:
                sys.stderr.write("arg3 u idiv neni symbol\n")
                sys.exit(53)
        else:
            sys.stderr.write("arg2 u idiv neni symbol\n")
            sys.exit(53)
    else:
        sys.stderr.write("arg1 u idiv neni variable\n")
        sys.exit(53)

def Lt(arg1,arg2,arg3):
    if arg1[0] == "var":
        if isSybol(arg2[0]):
            if isSybol(arg3[0]):
                var1 = arg2[1]
                var2 = arg3[1]
                if (arg2[0] == "int" or SelectTypeFromTable(arg2) == "int") and (arg3[0] == "int" or SelectTypeFromTable(arg3) == "int"):
                        if arg2[0] == "var":
                            var1 = SelectVariableFromTable(arg2)
                        if arg3[0] == "var":
                            var2 = SelectVariableFromTable(arg3)
                        var = int(var1) < int(var2)
                        var = str(var).lower()
                        SetValueInTable(arg1,arg1[1],var,ReturnType(var))
                elif (arg2[0] == "string" or SelectTypeFromTable(arg2) == "string") and (arg3[0] == "string" or SelectTypeFromTable(arg3) == "string"):
                        if arg2[0] == "var":
                            var1 = SelectVariableFromTable(arg2)
                        if arg3[0] == "var":
                            var2 = SelectVariableFromTable(arg3)
                        var = len(var1) < len(var2)
                        var = str(var).lower()
                        SetValueInTable(arg1,arg1[1],var,ReturnType(var))
                elif (arg2[0] == "bool" or SelectTypeFromTable(arg2) == "bool") and (arg3[0] == "bool" or SelectTypeFromTable(arg3) == "bool"):
                        if arg2[0] == "var":
                            var1 = SelectVariableFromTable(arg2)
                        if arg3[0] == "var":
                            var2 = SelectVariableFromTable(arg3)
                        if var1 == "false" and var2 == "true":
                            SetValueInTable(arg1,arg1[1],"true","bool")
                        else:
                            SetValueInTable(arg1,arg1[1],"false","bool")
                else:
                    sys.stderr.write("arg2 nebo arg3 u lt neni datovy typ\n")
                    sys.exit(53)
            else:
                sys.stderr.write("arg3 u lt neni symbol\n")
                sys.exit(53)
        else:
            sys.stderr.write("arg2 u lt neni symbol\n")
            sys.exit(53)
    else:
        sys.stderr.write("arg1 u lt neni variable\n")
        sys.exit(53)

def Gt(arg1,arg2,arg3):
    if arg1[0] == "var":
        if isSybol(arg2[0]):
            if isSybol(arg3[0]):
                var1 = arg2[1]
                var2 = arg3[1]
                if (arg2[0] == "int" or SelectTypeFromTable(arg2) == "int") and (arg3[0] == "int" or SelectTypeFromTable(arg3) == "int"):
                        if arg2[0] == "var":
                            var1 = SelectVariableFromTable(arg2)
                        if arg3[0] == "var":
                            var2 = SelectVariableFromTable(arg3)
                        var = int(var1) > int(var2)
                        var = str(var).lower()
                        SetValueInTable(arg1,arg1[1],var,ReturnType(var))
                elif (arg2[0] == "string" or SelectTypeFromTable(arg2) == "string") and (arg3[0] == "string" or SelectTypeFromTable(arg3) == "string"):
                        if arg2[0] == "var":
                            var1 = SelectVariableFromTable(arg2)
                        if arg3[0] == "var":
                            var2 = SelectVariableFromTable(arg3)
                        var = len(var1) > len(var2)
                        var = str(var).lower()
                        SetValueInTable(arg1,arg1[1],var,ReturnType(var))
                elif (arg2[0] == "bool" or SelectTypeFromTable(arg2) == "bool") and (arg3[0] == "bool" or SelectTypeFromTable(arg3) == "bool"):
                        if arg2[0] == "var":
                            var1 = SelectVariableFromTable(arg2)
                        if arg3[0] == "var":
                            var2 = SelectVariableFromTable(arg3)
                        if var1 == "true" and var2 == "false":
                            SetValueInTable(arg1,arg1[1],"true","bool")
                        else:
                            SetValueInTable(arg1,arg1[1],"false","bool")
                else:
                    sys.stderr.write("arg2 nebo arg3 u gt neni datovy typ\n")
                    sys.exit(53)
            else:
                sys.stderr.write("arg3 u gt neni symbol\n")
                sys.exit(53)
        else:
            sys.stderr.write("arg2 u gt neni symbol\n")
            sys.exit(53)
    else:
        sys.stderr.write("arg1 u gt neni variable\n")
        sys.exit(53)
def Eq(arg1,arg2,arg3):
    if arg1[0] == "var":
        if isSybol(arg2[0]):
            if isSybol(arg3[0]):
                var1 = arg2[1]
                var2 = arg3[1]
                if (arg2[0] == "int" or SelectTypeFromTable(arg2) == "int") and (arg3[0] == "int" or SelectTypeFromTable(arg3) == "int"):
                        if arg2[0] == "var":
                            var1 = SelectVariableFromTable(arg2)
                        if arg3[0] == "var":
                            var2 = SelectVariableFromTable(arg3)
                        var = int(var1) == int(var2)
                        var = str(var).lower()
                        SetValueInTable(arg1,arg1[1],var,ReturnType(var))
                elif (arg2[0] == "string" or SelectTypeFromTable(arg2) == "string") and (arg3[0] == "string" or SelectTypeFromTable(arg3) == "string"):
                        if arg2[0] == "var":
                            var1 = SelectVariableFromTable(arg2)
                        if arg3[0] == "var":
                            var2 = SelectVariableFromTable(arg3)
                        var = len(var1) == len(var2)
                        var = str(var).lower()
                        SetValueInTable(arg1,arg1[1],var,ReturnType(var))
                elif (arg2[0] == "bool" or SelectTypeFromTable(arg2) == "bool") and (arg3[0] == "bool" or SelectTypeFromTable(arg3) == "bool"):
                        if arg2[0] == "var":
                            var1 = SelectVariableFromTable(arg2)
                        if arg3[0] == "var":
                            var2 = SelectVariableFromTable(arg3)
                        if var1 == "true" and var2 == "true":
                            SetValueInTable(arg1,arg1[1],"true","bool")
                        elif var1 == "false" and var2 == "false":
                            SetValueInTable(arg1,arg1[1],"true","bool")
                        else:
                            SetValueInTable(arg1,arg1[1],"false","bool")
                else:
                    sys.stderr.write("arg2 nebo arg3 u eq neni datovy typ\n")
                    sys.exit(53)
            else:
                sys.stderr.write("arg3 u eq neni symbol\n")
                sys.exit(53)
        else:
            sys.stderr.write("arg2 u eq neni symbol\n")
            sys.exit(53)
    else:
        sys.stderr.write("arg1 u eq neni variable\n")
        sys.exit(53)
def And(arg1,arg2,arg3):
    if arg1[0] == "var":
        if isSybol(arg2[0]):
            if isSybol(arg3[0]):
                var1 = arg2[1]
                var2 = arg3[1]
                if (arg2[0] == "bool" or SelectTypeFromTable(arg2) == "bool") and (arg3[0] == "bool" or SelectTypeFromTable(arg3) == "bool"):
                        if arg2[0] == "var":
                            var1 = SelectVariableFromTable(arg2)
                        if arg3[0] == "var":
                            var2 = SelectVariableFromTable(arg3)
                        if var1 == var2:
                            if var1 == "true":
                                SetValueInTable(arg1,arg1[1],"true","bool")
                        else:
                            SetValueInTable(arg1,arg1[1],"false","bool")
                else:
                    sys.stderr.write("arg2 nebo arg3 u and neni bool\n")
                    sys.exit(53)
            else:
                sys.stderr.write("arg3 u and neni symbol\n")
                sys.exit(53)
        else:
            sys.stderr.write("arg2 u and neni symbol\n")
            sys.exit(53)
    else:
        sys.stderr.write("arg1 u and neni variable\n")
        sys.exit(53)

def Or(arg1,arg2,arg3):
    if arg1[0] == "var":
        if isSybol(arg2[0]):
            if isSybol(arg3[0]):
                var1 = arg2[1]
                var2 = arg3[1]
                if (arg2[0] == "bool" or SelectTypeFromTable(arg2) == "bool") and (arg3[0] == "bool" or SelectTypeFromTable(arg3) == "bool"):
                        if arg2[0] == "var":
                            var1 = SelectVariableFromTable(arg2)
                        if arg3[0] == "var":
                            var2 = SelectVariableFromTable(arg3)
                        if var1 == "true":
                            SetValueInTable(arg1,arg1[1],"true","bool")
                        elif var2 == "true":
                            SetValueInTable(arg1,arg1[1],"true","bool")
                        else:
                            SetValueInTable(arg1,arg1[1],"false","bool")
                else:
                    sys.stderr.write("arg2 nebo arg3 u or neni bool\n")
                    sys.exit(53)
            else:
                sys.stderr.write("arg3 u or neni symbol\n")
                sys.exit(53)
        else:
            sys.stderr.write("arg2 u or neni symbol\n")
            sys.exit(53)
    else:
        sys.stderr.write("arg1 u or neni variable\n")
        sys.exit(53)

def Not(arg1,arg2):
    if arg1[0] == "var":
        if isSybol(arg2[0]):
            var1 = arg2[1]
            if arg2[0] == "bool" or SelectTypeFromTable(arg2) == "bool":
                if arg2[0] == "var":
                            var1 = SelectVariableFromTable(arg2)
                if var1 == "true":
                        SetValueInTable(arg1,arg1[1],"false","bool")
                else:
                        SetValueInTable(arg1,arg1[1],"true","bool")
            else:
                sys.stderr.write("arg2 nebo arg3 u not neni bool\n")
                sys.exit(53)
        else:
            sys.stderr.write("arg2 u not neni symbol\n")
            sys.exit(53)
    else:
        sys.stderr.write("arg1 u not neni variable\n")
        sys.exit(53)

def Int2Char(arg1,arg2):
    if arg1[0] == "var":
        if isSybol(arg2[0]):
            if arg2[0] == "int":
                try:
                        var = chr(int(arg2[1]))
                        SetValueInTable(arg1,arg1[1],var,ReturnType(var))
                except ValueError:
                        sys.stderr.write("Neni validni ordinalni hodnota znaku v Unicode\n")
                        sys.exit(58)
            elif arg2[0]== "var" and SelectTypeFromTable(arg2) == "int":
                try:
                        var = chr(SelectTemporaryFromTable(arg2))
                        var = ConvertString(var)
                        SetValueInTable(arg1,arg1[1],var,ReturnType(var))
                except ValueError:
                        sys.stderr.write("Neni validni ordinalni hodnota znaku v Unicode\n")
                        sys.exit(53)
            else:
                sys.stderr.write("arg2 nebo arg3 u int2char neni int\n")
                sys.exit(53)
        else:
            sys.stderr.write("arg2 u int2char neni symbol\n")
            sys.exit(53)
    else:
        sys.stderr.write("arg1 u int2char neni variable\n")
        sys.exit(53)

def Stri2Int(arg1,arg2,arg3):
    if arg1[0] == "var":
        if isSybol(arg2[0]):
            if isSybol(arg3[0]):
                if (arg2[0] == "string" or SelectTypeFromTable(arg2) == "string") and (arg3[0] == "int" or SelectTypeFromTable(arg3) == "int"):
                        var1 = ConvertString(arg2[1])
                        var2 = arg3[1]
                        if arg2[0] == "var":
                            var1 = ConvertString(SelectVariableFromTable(arg2))
                        if arg3[0] == "var":
                            var2 = SelectVariableFromTable(arg3)
                        if int(var2) >= len(var1):
                            sys.stderr.write("Indexace mimo dany retezec\n")
                            sys.exit(58)
                        var = ord(var1[int(var2)])
                        SetValueInTable(arg1,arg1[1],var,ReturnType(var))
                else:
                    sys.stderr.write("arg2 nebo arg3 u stri2int neni int\n")
                    sys.exit(53)
            else:
                sys.stderr.write("arg3 u stri2int neni symbol\n")
                sys.exit(53)
        else:
            sys.stderr.write("arg2 u stri2int neni symbol\n")
            sys.exit(53)
    else:
        sys.stderr.write("arg1 u stri2int neni variable\n")
        sys.exit(53)

def CreateFrame():
    global temporarySymbolTable
    global frameCreated
    temporarySymbolTable = {}
    frameCreated = True
def PushFrame():
    global frameCreated
    global localSymbolTable
    global temporarySymbolTable
    if frameCreated:
        localSymbolTable.append(temporarySymbolTable)
        temporarySymbolTable = {}
        frameCreated = False
    else:
        sys.stderr.write("Pokus o pristup k nedefinovanemu ramci\n")
        sys.exit(55)
def PopFrame():
    global localSymbolTable
    global temporarySymbolTable
    if len(localSymbolTable) >= 1:
        temporarySymbolTable = localSymbolTable[-1]
        localSymbolTable.pop()
    else:
        sys.stderr.write("Na stacku lokalnich ramcu neni zadny ramec\n")
        sys.exit(55)
def Defvar(arg1):
    if(arg1[0] == "var"):
        global globalSymbolTable
        global localSymbolTable
        global temporarySymbolTable
        if arg1[2] == "GF":
            globalSymbolTable[arg1[1]] = ["",""]
        if arg1[2] == "LF":
            try:
                localSymbolTable[0][arg1[1]] = ["",""] 
            except IndexError:
                sys.stderr.write("Local frame doesnt exist\n")
                sys.exit(55)
        if arg1[2] == "TF":
            try:
                temporarySymbolTable[arg1[1]] = ["",""]
            except IndexError:
                sys.stderr.write("Temporary frame doesnt exist\n")
                sys.exit(55)
            
    else:
        sys.stderr.write("Defvar ocekava variable\n")
        sys.exit(53)
def Call(arg1,num):
    if(arg1[0] != "label"):
        sys.stderr.write("Call ocekava label\n")
        sys.exit(53)
    else:
        callStack.append(num)
        Jump(arg1)

def Return():
    global programCounter
    programCounter = int(callStack[-1])-1
    callStack.pop()

def Read(arg1,arg2):
    if arg1[0] == "var":
        if arg2[0] == "type":
            if arg2[1] == "int" or arg2[1] == "bool" or arg2[1] == "string":
                temp = ""
                try:
                    temp = str(eval(input("Input type "+arg2[1]+": ")))
                except NameError:
                    sys.stderr.write("Unexpected internal input error\n")
                    sys.exit()
                if arg2[1] == "bool":
                    if temp == "true":
                        SetValueInTable(arg1,arg1[1],"true","bool")
                    else:
                        SetValueInTable(arg1,arg1[1],"false","bool")
                if arg2[1] == "string":
                    SetValueInTable(arg1,arg1[1],temp,"string")
                if arg2[1] == "int":
                    if is_number(temp):
                        SetValueInTable(arg1,arg1[1],temp,"int")
                    else:
                        sys.stderr.write("Input nebylo cislo\n")
                        sys.exit(53)
            else:
                sys.stderr.write("Spatny typ u read\n")
                sys.exit(53)
def Write(arg1):
    if isSybol(arg1[0]):
        if arg1[0] == "var":
            print((ConvertString(SelectVariableFromTable(arg1))))
        else:
            print((ConvertString(arg1[1])))
    else:
        sys.stderr.write("Argument u write neni symbol\n")
        sys.exit(53)

def Concat(arg1,arg2,arg3):
    if arg1[0] == "var":
        var1 = arg2[1]
        var2 = arg3[1]
        if arg2[0] == "var":
            var1 = SelectVariableFromTable(arg2)
        if arg3[0] == "var":
            var2 = SelectVariableFromTable(arg3)
        var1 = ConvertString(var1)
        var2 = ConvertString(var2)
        var = str(var1)+str(var2)
        SetValueInTable(arg1,arg1[1],var,ReturnType(var))
    else:
        sys.stderr.write("Prvni argument neni variable\n")
        sys.exit(53)
def Strlen(arg1,arg2):
    if arg1[0] == "var":
        var1 = arg2[1]
        if (arg2[0] == "string" or SelectTypeFromTable(arg2) == "string"):
            if arg2[0] == "var":
                var1 = SelectVariableFromTable(arg2)
            var1 = ConvertString(var1)
            var = len(var1)
            SetValueInTable(arg1,arg1[1],var,ReturnType(var))
        else:
            sys.stderr.write("Argument u strlen neni string\n")
            sys.exit(53)
    else:
        sys.stderr.write("Prvni argument neni variable\n")
        sys.exit(53)
def GetChar(arg1,arg2,arg3):
    if arg1[0] == "var":
        var1 = arg2[1]
        var2 = arg3[1]
        if (arg2[0] == "string" or SelectTypeFromTable(arg2) == "string"):
            if (arg3[0] == "int" or SelectTypeFromTable(arg3) == "int"):
                if arg2[0] == "var":
                    var1 = SelectVariableFromTable(arg2)
                if arg3[0] == "var":
                    var2 = int(SelectVariableFromTable(arg3))
                if int(var2) >= len(var1):
                    sys.stderr.write("Indexace mimo dany retezec\n")
                    sys.exit(58)
                var1 = ConvertString(var1)
                var = var1[int(var2)]
                SetValueInTable(arg1,arg1[1],var,ReturnType(var))
            else:
                sys.stderr.write("arg3 u getchar neni typu int\n")
                sys.exit(53)
        else:
            sys.stderr.write("arg2 u getchar neni typu string\n")
            sys.exit(53)
    else:
        sys.stderr.write("Prvni argument neni variable\n")
        sys.exit(53)
def SetChar(arg1,arg2,arg3):
    if arg1[0] == "var":
        var1 = arg2[1]
        var2 = arg3[1]
        if (arg2[0] == "int" or SelectTypeFromTable(arg2) == "int"):
            if (arg3[0] == "string" or SelectTypeFromTable(arg3) == "string"):
                if arg2[0] == "var":
                    var1 = int(SelectVariableFromTable(arg2))
                else: 
                    var1 = int(var1)
                if arg3[0] == "var":
                    var2 = SelectVariableFromTable(arg3)
                var2 = ConvertString(var2)
                value = bytearray(SelectVariableFromTable(arg1), 'utf8')
                if int(var1) >= len(value):
                    sys.stderr.write("Indexace mimo dany retezec\n")
                    sys.exit(58)
                if var2 != "":
                    value[int(var1)] = ord(var2[0])
                    SetValueInTable(arg1,arg1[1],str(value.decode("utf-8")),ReturnType(value))
            else:
                sys.stderr.write("arg3 u setchar neni typu string\n")
                sys.exit(53)
        else:
            sys.stderr.write("arg2 u setchar neni typu int\n")
            sys.exit(53)
    else:
        sys.stderr.write("Prvni argument neni variable\n")
        sys.exit(53)

def is_number(s):
    try:
        float(s)
        return True
    except ValueError:
        pass

    try:
        import unicodedata
        unicodedata.numeric(s)
        return True
    except (TypeError, ValueError):
        pass
    return False

def FunctionType(arg1,arg2):
    if arg1[0] == "var":
        if arg2[0] == "var":
            var = SelectVariableFromTable(arg2)
            if is_number(var):
                SetValueInTable(arg1,arg1[1],"int",ReturnType(var))
            elif var == "true" or var == "false":
                SetValueInTable(arg1,arg1[1],"bool",ReturnType(var))
            else:
                SetValueInTable(arg1,arg1[1],"string",ReturnType(var))
        elif arg2[0] == "bool" or arg2[0] == "int" or arg2[0] == "string":
            SetValueInTable(arg1,arg1[1],arg2[0],ReturnType(var))
        else:
            SetValueInTable(arg1,arg1[1],"",ReturnType(var))
    else:
        sys.stderr.write("Prvni argument neni variable\n")
        sys.exit(53)
def Label(arg1,order):
    if arg1[0] == "label":
        if not arg1[1] in labelTable:
            labelTable[arg1[1]] = order
        else:
            sys.stderr.write("Label jiz existuje\n")
            sys.exit(52)
    else:
        sys.stderr.write("Prvni argument neni label\n")
        sys.exit(53)
def Jump(arg1):
    global programCounter
    if arg1[0] == "label":
        if any(arg1[1] in x for x in labelTable):
            programCounter = int(labelTable[arg1[1]])-1
        else:
            sys.stderr.write("Neexistuje zadny takovy label\n")
            sys.exit(52)
    else:
        sys.stderr.write("Prvni argument neni label\n")
        sys.exit(53)
def Jumpifeq(arg1,arg2,arg3):
    global programCounter
    if arg1[0] == "label":
        equal = False
        if arg2[0] == "string" and arg2[0] == arg3[0]:
            if arg2[1] == arg3[1]:
                equal = True
        elif arg2[0] == "int" and arg2[0] == arg3[0]:
            if arg2[1] == arg3[1]:
                equal = True
        elif arg2[0] == "bool" and arg2[0] == arg3[0]:
            if arg2[1] == arg3[1]:
                equal = True
        else:
            tmparg = arg3[1]
            if arg3[0]== "var":
                tmparg = SelectVariableFromTable(arg3)
            if is_number(SelectVariableFromTable(arg2)) and is_number(tmparg):
                if int(SelectVariableFromTable(arg2)) == int(tmparg):
                    equal = True
            elif SelectVariableFromTable(arg2)=="true" or SelectVariableFromTable(arg2)=="false":
                if tmparg=="true" or tmparg=="false":
                    if SelectVariableFromTable(arg2) == tmparg:
                        equal = True
            elif SelectTypeFromTable(arg2) == "string" and ReturnType(tmparg) == "string":
                if SelectVariableFromTable(arg2) == tmparg:
                        equal = True
            else:
                sys.stderr.write("Argumenty nejsou stejneho typu\n")
                sys.exit(53)
        if equal:
            if any(arg1[1] in x for x in labelTable):
                programCounter = int(labelTable[arg1[1]])-1
            else:
                sys.stderr.write("Neexistuje zadny takovy label\n")
                sys.exit(52)
    else:
        sys.stderr.write("Prvni argument neni label\n")
        sys.exit(53)

def Jumpifneq(arg1,arg2,arg3):
    global programCounter
    if arg1[0] == "label":
        nequal = False
        if arg2[0] == "string" and arg2[0] == arg3[0]:
            if arg2[1] != arg3[1]:
                nequal = True
        elif arg2[0] == "int" and arg2[0] == arg3[0]:
            if arg2[1] != arg3[1]:
                nequal = True
        elif arg2[0] == "bool" and arg2[0] == arg3[0]:
            if arg2[1] != arg3[1]:
                nequal = True
        else:
            tmparg = arg3[1]
            if arg3[0] == "var":
                tmparg = SelectVariableFromTable(arg3)
            if is_number(SelectVariableFromTable(arg2)) and is_number(tmparg):
                if int(SelectVariableFromTable(arg2)) != int(tmparg):
                    nequal = True
            elif SelectVariableFromTable(arg2)=="true" or SelectVariableFromTable(arg2)=="false":
                if tmparg=="true" or tmparg=="false":
                    if SelectVariableFromTable(arg2) != tmparg:
                        nequal = True
            elif SelectTypeFromTable(arg2) == "string" and ReturnType(tmparg) == "string":
                if SelectVariableFromTable(arg2) != tmparg:
                        nequal = True
            else:
                sys.stderr.write("Argumenty nejsou stejneho typu\n")
                sys.exit(53)
        if nequal:
            if any(arg1[1] in x for x in labelTable):
                programCounter = int(labelTable[arg1[1]])-1
            else:
                sys.stderr.write("Neexistuje zadny takovy label\n")
                sys.exit(52)
    else:
        sys.stderr.write("Prvni argument neni label\n")
        sys.exit(53)

def Pushs(arg1):
    if arg1[0] == "var":
        dataStack.append(SelectVariableFromTable(arg1))
    else:
        dataStack.append(arg1[1])

def Pops(arg1):
    if len(dataStack) == 0:
        sys.stderr.write("Datovy zasobnik je prazdny\n")
        sys.exit(56)
    if arg1[0] == "var":
        SetValueInTable(arg1,arg1[1],dataStack[-1],ReturnType(dataStack[-1]))
        dataStack.pop()
    else:
        sys.stderr.write("Pops ocekava variable jako argument\n")
        sys.exit(53)

def Dprint(arg1):
    if isSybol(arg1[0]):
        if arg1[0] == "var":
            sys.stderr.write(SelectVariableFromTable(arg1))
            if SelectTypeFromTable(arg1) != "string":
                sys.stderr.write("\n")
        else:
            sys.stderr.write(arg[1]+"\n")
    else:
        sys.stderr.write("Prvni argument neni symbol\n")
        sys.exit(53)

def Break():
    global globalSymbolTable
    global localSymbolTable
    global temporarySymbolTable
    global programCounter
    global instrDone
    sys.stderr.write("PROGRAM COUNTER: "+str(programCounter)+"\n")
    sys.stderr.write("INSTRUCTIONS DONE: "+str(instrDone)+"\n")
    sys.stderr.write("GLOBAL:\n")
    for x in globalSymbolTable:
        varType = str(globalSymbolTable[x][1])
        if varType == "":
            varType = "NONE"
        sys.stderr.write("[ "+varType+" "+x+" : "+str(globalSymbolTable[x][0])+" ]\n")
    print("Lenght: "+str(len(globalSymbolTable)))
    sys.stderr.write("LOCAL:\n")
    i = 0
    while i< len(localSymbolTable):
        for x in localSymbolTable[i]:
            varType = str(localSymbolTable[i][x][1])
            if varType == "":
                varType = "NONE"
            sys.stderr.write("[ "+varType+" "+x+" : "+str(localSymbolTable[i][x][0])+" ]\n")
        i= i+1
    print("Lenght: "+str(len(localSymbolTable)))
    sys.stderr.write("TEMPORARY:\n")
    for x in temporarySymbolTable:
        varType = str(temporarySymbolTable[x][1])
        if varType == "":
            varType = "NONE"
        sys.stderr.write("[ "+varType+" "+x+" : "+str(temporarySymbolTable[x][0])+" ]\n")
    print("Lenght: "+str(len(temporarySymbolTable)))

def InstructinSelector(inst):
    if inst.instr == "MOVE":
        Move(inst.arg1,inst.arg2)
    if inst.instr == "CREATEFRAME":
        CreateFrame()
    if inst.instr == "PUSHFRAME":
        PushFrame()
    if inst.instr == "POPFRAME":
        PopFrame()
    if inst.instr == "DEFVAR":
        Defvar(inst.arg1)
    if inst.instr == "CALL":
        Call(inst.arg1,inst.numb)
    if inst.instr == "RETURN":
        Return()
    if inst.instr == "PUSHS":
        Pushs(inst.arg1)
    if inst.instr == "POPS":
        Pops(inst.arg1)
    if inst.instr == "ADD":
        Add(inst.arg1,inst.arg2,inst.arg3)
    if inst.instr == "SUB":
        Sub(inst.arg1,inst.arg2,inst.arg3)
    if inst.instr == "MUL":
        Mul(inst.arg1,inst.arg2,inst.arg3)
    if inst.instr == "IDIV":
        IDiv(inst.arg1,inst.arg2,inst.arg3)
    if inst.instr == "LT":
        Lt(inst.arg1,inst.arg2,inst.arg3)
    if inst.instr == "GT":
        Gt(inst.arg1,inst.arg2,inst.arg3)
    if inst.instr == "EQ":
        Eq(inst.arg1,inst.arg2,inst.arg3)
    if inst.instr == "AND":
        And(inst.arg1,inst.arg2,inst.arg3)
    if inst.instr == "OR":
        Or(inst.arg1,inst.arg2,inst.arg3)
    if inst.instr == "NOT":
        Not(inst.arg1,inst.arg2)
    if inst.instr == "INT2CHAR":
        Int2Char(inst.arg1,inst.arg2)
    if inst.instr == "STRI2INT":
        Stri2Int(inst.arg1,inst.arg2,inst.arg3)
    if inst.instr == "READ":
        Read(inst.arg1,inst.arg2)
    if inst.instr == "WRITE":
        Write(inst.arg1)
    if inst.instr == "CONCAT":
        Concat(inst.arg1,inst.arg2,inst.arg3)
    if inst.instr == "STRLEN":
        Strlen(inst.arg1,inst.arg2)
    if inst.instr == "GETCHAR":
        GetChar(inst.arg1,inst.arg2,inst.arg3)
    if inst.instr == "SETCHAR":
        SetChar(inst.arg1,inst.arg2,inst.arg3)
    if inst.instr == "TYPE":
        FunctionType(inst.arg1,inst.arg2)
    #if inst.instr == "LABEL":
    #    Label(inst.arg1,inst.numb)
    if inst.instr == "JUMP":
        Jump(inst.arg1)
    if inst.instr == "JUMPIFEQ":
        Jumpifeq(inst.arg1,inst.arg2, inst.arg3)
    if inst.instr == "JUMPIFNEQ":
        Jumpifneq(inst.arg1,inst.arg2, inst.arg3)
    if inst.instr == "DPRINT":
        Dprint(inst.arg1)
    if inst.instr == "BREAK":
        Break()
def main():
    import getopt
    import xml.etree.ElementTree as ET
    name = ""
    lst =[]
    global programCounter
    global instrDone
    try:
        opts, args = getopt.getopt(sys.argv[1:], "ho:", ["help", "source="])
    except getopt.GetoptError as err:
        # print help information and exit:
        print((str(err)))  # will print something like "option -a not recognized"
        usage()
        sys.exit(2)
    for o, a in opts:
        if o in ("-h", "--help"):
            #usage()
            sys.exit()
        elif o in ("-o", "--source"):
            name = a
        else:
            assert False, "unhandled option"
        #print(name)
    if name == "":
        sys.exit(0)
    try:
        tree = ET.parse(name)
    except ET.ParseError:
        sys.stderr.write("Error: Xml file is not parsable.\n")
        sys.exit(31)
    
    root = tree.getroot()
    #print(root.tag)              #program
    #print(root.attrib)           #language = IPPcode18
    instructionList = []
    for instr in root.iter('instruction'):
        arg1=[]
        arg2=[]
        arg3=[]
        for arg in instr.iter('arg1'):
            arg1.append(str(arg.attrib["type"]))
            if arg.text:
                temp = str(arg.text)
                arg1.append(str(temp))
            else:
                arg1.append("")
        for arg in instr.iter('arg2'):
            arg2.append(str(arg.attrib["type"]))
            if arg.text:
                temp = str(arg.text)
                arg2.append(str(temp))
            else:
                arg2.append("")
        for arg in instr.iter('arg3'):
            arg3.append(str(arg.attrib["type"]))
            if arg.text:
                temp = str(arg.text)
                arg3.append(str(temp))
            else:
                arg3.append("")
        inst = Instruction(instr.attrib["opcode"],instr.attrib["order"],arg1,arg2,arg3)
        instructionList.append(inst)
    while programCounter< len(instructionList):
        if instructionList[programCounter].instr == "LABEL":
            Label(instructionList[programCounter].arg1,instructionList[programCounter].numb)
        programCounter=int(programCounter)+1
        instrDone = int(instrDone)+1
        
    #print "Labels:"
    #print labelTable
    programCounter = 0
    while programCounter< len(instructionList):
        #ins.writeInstruction()
        InstructinSelector(instructionList[programCounter])
        programCounter=int(programCounter)+1
        instrDone = int(instrDone)+1
if __name__ == "__main__":
    main()