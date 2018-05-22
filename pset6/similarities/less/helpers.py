from nltk.tokenize import sent_tokenize

def lines(a, b):
    """Return lines in both a and b"""
    alist = set(a.splitlines())
    blist = set(b.splitlines())
    return alist & blist


def sentences(a, b):
    """Return sentences in both a and b"""

    alist = set(sent_tokenize(a, language='english'))
    blist = set(sent_tokenize(b, language='english'))

    return alist & blist

def get_substings(str, n):
    substringlist = []
    str = str.replace('\n', '')

    for i in range(len(str) - n + 1):
        substringlist.append(str[i:i + n])

    return substringlist

def substrings(a, b, n):
    """Return substrings of length n in both a and b"""
    alist = set(get_substings(a, n))
    blist = set(get_substings(b, n))
    
    return alist & blist