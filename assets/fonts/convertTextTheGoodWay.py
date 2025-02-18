import pyperclip

stringToConvert = "équiper"

def isGoodLetter(letter: str):
    return letter.isascii() and (letter.isalpha() or letter in ".,;:!?-")

def convert(string):
    newStr = ""
    for letter in string:
        if not isGoodLetter(letter):
            newStr += f"\\x{hex(ord(letter)).upper()[2:]}\\"
        else:
            newStr += letter
    return newStr


converted = convert(stringToConvert)
print(converted)
pyperclip.copy(converted)