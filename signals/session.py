#!/usr/bin/env python3
"""Replay a terminal session in an interactive bash on a pseudo-terminal.

Control-C, Control-Z and typed input must reach the programs through a real
terminal, so that the terminal driver sends the signals to the foreground
process group, as in the lecture. Usage: ./session.py NAME (see SESSIONS).

Each step is (delay, text): wait `delay` seconds, then type `text`.
"""
import fcntl
import os
import pty
import select
import signal
import struct
import sys
import termios
import time

CTRL_C, CTRL_Z = '\x03', '\x1a'

SESSIONS = {
    'alarm': [(0.5, './alarm\n'), (4.0, 'echo $?\n')],
    'handler': [(0.5, './handler\n'), (4.0, '')],
    'critical': [(0.5, './critical\n'), (1.0, CTRL_C),
                 (0.5, './critical\n'), (4.0, CTRL_C), (6.0, '')],
    'mask': [(0.5, './mask\n'), (1.0, CTRL_C), (3.0, 'echo $?\n')],
    'limit': [(0.5, './limit 5 ls *.c\n'), (1.0, './limit 4 sleep 100\n'),
              (5.0, '')],
    'pulse': [(0.5, './pulse\n'), (10.5, '')],
    'pgrp1': [(0.5, './pgrp1\n'), (1.0, CTRL_C), (0.5, '')],
    'pgrp2': [(0.5, './pgrp2\n'), (0.5, CTRL_C), (3.5, '')],
    'pgrp3': [(0.5, './pgrp3\n'), (1.5, '')],
    'segv': [(0.5, './segv\n'), (0.5, 'echo $?\n')],
    'jobs': [(0.5, 'sleep 100\n'), (1.0, CTRL_Z), (0.5, 'bg\n'),
             (0.5, 'kill -STOP %1\n'), (0.5, 'jobs -l\n'),
             (0.5, 'kill -CONT %1\n'), (0.5, 'kill -TERM %1\n'),
             (0.5, 'jobs -l\n')],
    'numbers': [(0.3, 'kill -l | head -7\n')],
    'keys': [(0.3, "stty -a | grep -Eo '(intr|quit|susp) = [^;]+'\n")],
    'procsig': [(0.5, './critical > /dev/null & C=$!; sleep 4\n'),
                (4.5, './handler > /dev/null & H=$!; sleep 0.5; grep -E "^Sig(Ign|Cgt)" /proc/$C/status /proc/$H/status\n'),
                (1.0, 'wait\n'), (5.0, '')],
    'semantics': [(0.3, 'gcc handler.c -o h-posix; gcc -D_DEFAULT_SOURCE handler.c -o h-default\n'),
                  (1.5, "for p in h-posix h-default; do strace -e trace=rt_sigaction ./$p 2>&1 | grep -o 'sa_flags=[A-Z_|]*' | head -1; done\n"),
                  (7.5, '')],
    'pstty': [(0.3, 'sleep 100 &\n'), (0.5, 'ps -o pid,ppid,pgid,sid,tpgid,stat,comm\n'),
              (0.5, 'kill %1\n')],
    'strace': [(0.5, 'strace -e trace=rt_sigaction,alarm,pause ./handler\n'), (4.5, '')],
    'timeout': [(0.3, 'timeout 2 sleep 10; echo $?\n'), (2.5, 'timeout -s KILL 1 sleep 10; echo $?\n'), (1.5, '')],
}


def run(steps):
    pid, fd = pty.fork()
    if pid == 0:
        # Python ignores SIGPIPE and SIGXFSZ; restore the defaults that a
        # login shell would pass on, and give long commands room on one line.
        signal.signal(signal.SIGPIPE, signal.SIG_DFL)
        signal.signal(signal.SIGXFSZ, signal.SIG_DFL)
        winsize = struct.pack('HHHH', 50, 200, 0, 0)
        fcntl.ioctl(0, termios.TIOCSWINSZ, winsize)
        os.environ.update(PS1='$ ', TERM='dumb', HISTFILE='/dev/null')
        os.execlp('bash', 'bash', '--norc', '--noprofile', '-i')
    out = bytearray()

    def drain(seconds):
        end = time.time() + seconds
        while (left := end - time.time()) > 0:
            ready, _, _ = select.select([fd], [], [], left)
            if ready:
                try:
                    out.extend(os.read(fd, 4096))
                except OSError:
                    return

    drain(0.5)
    for delay, text in steps:
        drain(delay)
        os.write(fd, text.encode())
    drain(1.0)
    os.write(fd, b'exit\n')
    drain(0.5)
    text = out.decode(errors='replace').replace('\r\n', '\n').replace('\r', '')
    lines = text.split('\n')
    while lines and lines[-1].strip() in ('', '$', '$ exit', 'exit'):
        lines.pop()
    return '\n'.join(lines).lstrip('\n').expandtabs()


if __name__ == '__main__':
    for name in sys.argv[1:] or SESSIONS:
        print(f'=== {name}')
        print(run(SESSIONS[name]))
