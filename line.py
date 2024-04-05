from startswith import Startswith

class Line:
    def __init__(self, line: str, finding_value: bool) -> None:
        self.instruction = 0b11110 # default to exit with status 0
        self.register = 0
        self.address = 0
        self.__combine()
        
        if finding_value:
            self.__value(line)
        else:
            self.__compile(line)

    def __compile(self, line: str) -> None:
        sw_whole_line = Startswith(line)
        match sw_whole_line:
            case "move>":   self.instruction = 0b00000 # move>regAddresss [from reg to Addresss]
            case "move<":   self.instruction = 0b00001 # move<regAddresss [from Addresss to reg]
            case "put<<":   self.instruction = 0b00010 # put<<reg________Value... [value into reg]
            case "put<":    self.instruction = 0b00011 # put<____AddresssValue... [value into Addresss]
    
            case "move>>":  self.instruction = 0b00100 # mov>>regAddresss [from address in (char)reg to Address]
            case "move<<":  self.instruction = 0b00101 # mov<<reg____.reg [from address in (char).reg to reg]
            case "shift<<": self.instruction = 0b00110 # <<___reg.reg-reg [<<, like below]
            case "shift>>": self.instruction = 0b00111 # >>___

            case "add":     self.instruction = 0b01000 # add__reg.reg-reg [put (.reg + -reg) into reg]
            case "sub":     self.instruction = 0b01001 # sub__
            case "mult":    self.instruction = 0b01010 # mult_
            case "div":     self.instruction = 0b01011 # div__

            case "and":     self.instruction = 0b01100 # AND__reg.reg-reg [&, like above]
            case "or":      self.instruction = 0b01101 # OR___
            case "nand":    self.instruction = 0b01110 # NAND_
            case "nor":     self.instruction = 0b01111 # NOR__

            case "drawrv":  self.instruction = 0b10000 # draw_reg.reg_xxx [set the pixel at [.reg][x] to the colour in reg]
            case "drawvr":  self.instruction = 0b10001 # draw_reg_yyy-reg [set the pixel at [y][-reg] to the colour in reg]
            case "ifgoto":  self.instruction = 0b10010 # if___regCodeLine [if (reg) goto CodeLine]
            # case "": self.instruction = 0b10011 #

            case "push":    self.instruction = 0b11000 # push_reg________ [push the value in reg onto the stack]
            case "pop":     self.instruction = 0b11001 # pop__reg________ [pop the first value off of the stack and put it into reg]
            case "drawv":   self.instruction = 0b11010 # drawvreg_yyy_xxx [set the pixel at [y][x] to the colour in reg]
            case "drawr":   self.instruction = 0b11011 # drawrreg.reg-reg [set the pixel at [.reg][-reg] to the colour in reg]

            case "if":      self.instruction = 0b11100 # nIf__regCodeLine [if (!reg) goto CodeLine]
            case "goto":    self.instruction = 0b11101 # GoTo____CodeLine [goto CodeLine]
            case "exitv":   self.instruction = 0b11110 # exitv___value___ [exit with code value___]
            case "exitr":   self.instruction = 0b11111 # exitrreg________ [exit with code in reg]
            case _: raise BadLine()
        
        locations = sw_whole_line.without_start()
        try:
            self.register = eval(f"0o{locations[0]}")
            self.address = eval(f"0x{locations[1:][:2]}")
        except: raise BadLine()

        self.__combine()

        if sw_whole_line.start.startswith("put<"):
            raise RequiresValue(self)

    def __value(self, line: str) -> None:
        try: self.full = [ eval(f"0x{line[:2]}"), eval(f"0x{line[2:4]}") ]
        except: raise BadLine()

    def __combine(self) -> None:
        self.full = [
            (self.instruction << 3) + self.register,
            self.address
        ]
    
    def to_bytes(self) -> bytes:
        # Python thinks that this puts the bytes in the wrong order, but somehow this fixes it
        return int.to_bytes(self.full[1]) + int.to_bytes(self.full[0])


class RequiresValue(Exception):
    def __init__(self, line: Line) -> None:
        super().__init__()
        self.line = line

class BadLine(Exception):
    def __init__(self) -> None:
        super().__init__()
