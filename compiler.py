#!/bin/python3.11
from sys import argv
import re
from line import Line, BadLine, RequiresValue

BYTE = 256

def main():
    lines = getFile(argv[1])
    preprocessed = preprocess(lines)
    compiled = compile(preprocessed)
    write_out(compiled)

def getFile(filename: str) -> list[str]:
    with open(filename) as f:
        return f.read().lower().split("\n")

def preprocess(lines: list[str]) -> list[str]:
    preprocessed = []
    in_ml_comment = False
    for line in lines:
        line = re.sub(r"\s","",line)
        line = line.split("//")[0]
        if (line[:2] == "/*"): in_ml_comment = True
        if (in_ml_comment):
            if (line[-2:] == "*/"): in_ml_comment = False
            line = ""
        if (not line): continue
        preprocessed.append(line)
    return preprocessed

def compile(lines: list[str]) -> list[bytes]:
    compiled = []
    finding_value = False
    for line in lines:
        if len(compiled) > BYTE: break
        try:
            l = Line(line, finding_value)
            finding_value = False
        except BadLine:
            continue
        except RequiresValue as e:
            l = e.line
            finding_value = True
        
        compiled.append(l.to_bytes())

    return compiled

def write_out(lines: list[bytes]):
    out = b''
    for line in lines:
        out += line
    with open("b.piout", "bw") as f:
        f.write(out)


if __name__ == "__main__":
    main()
