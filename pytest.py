class classTest:
    def __init__(self, name):
        self.myName = name

    def printMyName(self):
        print 'My name is %s' % (self.myName)

def add(a,b):
    print 'a= ' + str(a)
    print 'b= ' + str(b)
    print 'ret= ' + str(a + b)

def foo(a):
    print 'a= ' + str(a)
    print 'ret= ' + str(a*a)