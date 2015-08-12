#!/usr/bin/env python
import sys,os,thread,threading



class stress(threading.Thread):

    def __init__ (self,command):
        threading.Thread.__init__(self)
        self.command = command

    def run(self):
        p = os.popen(self.command,"r")
        while 1:
            line = p.readline()
            if not line: break
            print line


def main():
    arguments = sys.argv
    print(arguments[1])
    if(len(arguments)>5):
        print ("La forma correcta es ./stress -n <cantidad-hilos> httpclient <parametros del cliente>")

    threads = []
    for num in range(1,int(arguments[1],10)):
        thread = stress("./"+arguments[2]+" "+"-u "+arguments[3])
        thread.start()
        threads.append(thread)

    for thread in threads:
        thread.join
    return 0;

if __name__ == "__main__":
    main()
