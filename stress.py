#!/usr/bin/env python
import sys,os



def printCall(command):
        p = os.popen(command,"r")
        while 1:
            line = p.readline()
            if not line: break
            print line

def stress(threads,client,url):
    while(threads>0):
        threads-=1
        printCall("./"+client+" "+"-u "+url)




def main():
    arguments = sys.argv
    print(arguments[1])
    if(len(arguments)>5):
        print ("La forma correcta es ./stress -n <cantidad-hilos> httpclient <parametros del cliente>")
    stress(int(arguments[1],10),arguments[2],arguments[3])

if __name__ == "__main__":
    main()
