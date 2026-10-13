#!/usr/bin/env python3
"""Replay a terminal session in an interactive bash on a pseudo-terminal.

Background jobs (the chef, the Python tasks) run in the same shell as the
commands that inspect them, so job-control messages appear as in the lecture.
Usage: ./session.py [NAME...] (see SESSIONS); compile the programs first.

Each step is (delay, text): wait `delay` seconds, then type `text`. SETUP runs
before a session, without being shown, to create the files it starts from.
"""
import fcntl
import os
import pty
import select
import signal
import struct
import subprocess
import sys
import termios
import time

CTRL_D = '\x04'

SETUP = {
    'chef': 'rm -f recipe',
}
DIRS = {}
SIZES = {}

SESSIONS = {
    'hello': [(0.3, './hello\n'), (0.5, './hello\n')],
    'args': [(0.3, './args\n'), (0.5, './args\n'), (0.5, './args fixed\n')],
    'busywork': [(0.3, 'time ./busywork\n'), (2.5, '')],
    'share': [(0.3, './share\n')],
    'race': [(0.3, './race\n'), (0.5, './race\n'), (0.5, './race lock\n'), (1.0, '')],
    'prodcons': [(0.3, './prodcons\n')],
    'bench': [(0.3, './bench\n'), (4.0, './bench 2000 1024\n'), (60.0, '')],
    'chef': [(0.3, './chef 3 &\n'),
             (0.5, './cook > /dev/null & ./cook > /dev/null & sleep 0.5; ps -L -o pid,lwp,nlwp,stat,args -C chef\n'),
             (1.0, 'sleep 3; ps -L -o pid,lwp,nlwp,stat,args -C chef\n'),
             (3.8, 'kill %1; wait\n')],
    'tasks': [(0.3, 'python3 tasks.py & sleep 1; ps -L -o pid,lwp,nlwp,comm -p $!\n'),
              (1.8, 'ls /proc/$!/task; grep -E "^(Tgid|Pid|Threads)" /proc/$!/status\n'),
              (0.8, 'wait\n'), (8.0, '')],
    'cpus': [(0.3, './busywork > /dev/null & sleep 0.5; ps -L -o pid,lwp,psr,pcpu,stat,comm -p $!\n'),
             (1.5, 'wait\n'), (2.0, '')],
    'strace': [(0.3, 'strace -q -e trace=clone ./share 2>&1 > /dev/null | grep -o "flags=[^,]*"\n'),
               (1.0, 'strace -q -e trace=clone3 ./share 2>&1 > /dev/null | grep -o "flags=[^,]*" | tr "|" "\\n"\n'),
               (1.0, '')],
    'tsan': [(0.3, 'gcc -g -fsanitize=thread -fdebug-prefix-map=$PWD=. -pthread race.c -o race-tsan\n'),
             (3.0, './race-tsan 2>&1 | head -11\n'), (2.0, './race-tsan lock\n'), (3.0, '')],
    'ns': [(0.3, 'ls /proc/self/ns\n'),
           (0.5, 'unshare -Ur --uts sh -c "hostname demo; hostname"; hostname\n'),
           (0.8, 'unshare -Ur --pid --fork --mount-proc ps -e\n'), (1.0, '')],
}

def run(name):
    setup = SETUP.get(name)
    if setup:
        subprocess.run(setup, shell=True, check=True)
    rows, cols = SIZES.get(name, (50, 200))
    pid, fd = pty.fork()
    if pid == 0:
        # Python ignores SIGPIPE and SIGXFSZ; restore the defaults that a
        # login shell would pass on, and give long commands room on one line.
        signal.signal(signal.SIGPIPE, signal.SIG_DFL)
        signal.signal(signal.SIGXFSZ, signal.SIG_DFL)
        fcntl.ioctl(0, termios.TIOCSWINSZ, struct.pack('HHHH', rows, cols, 0, 0))
        os.closerange(3, 1024)  # the programs must see the first free fd, 3
        os.chdir(DIRS.get(name, '.'))
        os.environ.update(PS1='$ ', TERM='dumb', HISTFILE='/dev/null')
        # -o notify: report a killed server at once, not at the next prompt.
        os.execlp('bash', 'bash', '--norc', '--noprofile', '-o', 'notify', '-i')
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
    for delay, text in SESSIONS[name]:
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
        print(run(name))
