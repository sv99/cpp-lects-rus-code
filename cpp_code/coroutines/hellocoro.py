#!/usr/bin/python3

def hello():
    print("hello, ")
    yield
    print("world!")
    yield

task = hello()
next(task)
next(task)
