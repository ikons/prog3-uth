#!/usr/bin/env python3
"""The lecture's thread1.py: two tasks in two threads of one process."""
import os
import threading
import time


def task(n):
    me = threading.current_thread()
    print(f"Task {n} assigned to thread: {me.name}, tid {threading.get_native_id()}")
    print(f"ID of process running task {n}: {os.getpid()}")
    time.sleep(8)  # time enough to look at the threads with ps -L


if __name__ == "__main__":
    print(f"ID of process running main program: {os.getpid()}")
    print(f"Main thread name: {threading.current_thread().name}")

    t1 = threading.Thread(target=task, args=(1,), name='t1')
    t2 = threading.Thread(target=task, args=(2,), name='t2')
    t1.start()
    t2.start()
    t1.join()
    t2.join()
