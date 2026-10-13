#!/usr/bin/env python3
"""Replay a terminal session in an interactive bash on a pseudo-terminal.

Servers run in the background of the same shell as their clients, so the
job-control messages ([1] 4698, Terminated) appear as in the lecture.
Usage: ./session.py [NAME...] (see SESSIONS); compile the programs first.

Each step is (delay, text): wait `delay` seconds, then type `text`. SETUP runs
before a session, without being shown, to create the files it starts from.
"""
import fcntl
import os
import re
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
    'cookfirst': 'rm -f recipe',
    'strace': 'rm -f recipe',
    'nc': 'rm -f recipe',
}
DIRS = {}
SIZES = {'ss': (50, 96)}

SESSIONS = {
    'chef': [(0.3, './chef &\n'), (0.5, './cook\n'), (0.5, './cook\n'),
             (0.5, 'ls -l recipe\n'), (0.3, 'kill %1; wait\n')],
    'children': [(0.3, './chef 3 &\n'),
                 (0.5, './cook > /dev/null & ./cook > /dev/null & sleep 0.5; ps -o pid,ppid,stat,args -C chef\n'),
                 (1.0, 'sleep 3; kill %1; wait\n'), (3.8, '')],
    'cookfirst': [(0.3, './cook & sleep 2; ./chef & wait %1\n'), (4.5, 'kill %2; wait\n')],
    'inettime': [(0.3, './daytimed 2> daytimed.log &\n'), (0.5, './inettime\n'),
                 (0.5, 's\n'), (0.8, 'localhost\n'), (0.8, '127.0.0.1\n'),
                 (0.8, 'time.nist.gov\n'), (2.5, 'nosuch.invalid\n'), (1.0, 'q\n'),
                 (0.5, 'cat daytimed.log\n'), (0.3, 'kill %1; wait\n')],
    'inuse': [(0.3, './daytimed 2> /dev/null &\n'), (0.5, './daytimed\n'),
              (0.5, './inettime 7\n'), (0.5, 'localhost\n'), (0.8, 'q\n'),
              (0.3, 'kill %1; wait\n')],
    'web': [(0.3, './web-server > web.log &\n'), (0.5, 'curl -i http://localhost:8080/\n'),
            (0.8, 'curl -sI http://localhost:8080/picture.png\n'),
            (0.8, 'curl -i http://localhost:8080/nothing.html\n'),
            (0.8, 'curl -i --path-as-is http://localhost:8080/../../etc/passwd\n'),
            (0.8, 'kill %1; wait\n'), (0.5, 'cat web.log\n')],
    'webclient': [(0.3, './web-server > /dev/null &\n'),
                  (0.5, './web-client localhost 8080 index.html\n'),
                  (0.8, './web-client localhost 8080 picture.png | head -5 | cat -A\n'),
                  (0.8, 'kill %1; wait\n')],
    'shm': [(0.3, './shmserver & sleep 0.3; ipcs -m\n'),
            (0.8, './shmclient; sleep 1.2\n'), (1.8, 'ipcs -m\n')],
    'somaxconn': [(0.3, 'cat /proc/sys/net/core/somaxconn\n')],
    'services': [(0.3, 'grep -E "^(daytime|ssh|smtp|domain|http|https|http-alt)\\s+[0-9]+/tcp" /etc/services\n')],
    'shmleak': [(0.3, './shmserver > /dev/null & sleep 0.3; kill -9 %1; wait\n'),
                (0.5, 'ipcs -m\n'), (0.4, 'ipcrm -M 5678\n'), (0.4, 'ipcs -m\n')],
    'race': [(0.3, './race\n'), (1.5, './race\n'), (1.5, './race lock\n'), (3.0, '')],
    'ss': [(0.3, './chef & ./web-server > /dev/null & ./daytimed 2> /dev/null &\n'),
           (0.6, 'ss -xlp | grep -E "Netid|recipe"\n'), (0.5, 'ss -tlnp\n'),
           (0.5, 'kill %1 %2 %3; wait\n'), (0.8, '')],
    'strace': [(0.3, './chef &\n'), (0.5, 'strace -e trace=socket,connect ./cook\n'),
               (1.0, 'strace -e trace=read ./cook 2>&1 | grep -c "^read(3"\n'),
               (1.0, 'kill %1; wait\n')],
    'nc': [(0.3, './chef & ./daytimed 2> /dev/null &\n'),
           (0.5, 'nc -U recipe < /dev/null | tr "\\0" "\\n"\n'),
           (0.5, 'nc localhost 13 < /dev/null\n'),
           (0.5, 'cat < /dev/tcp/time.nist.gov/13\n'), (2.5, 'kill %1 %2; wait\n')],
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
    text = re.sub(r'\x1b\[[0-9;]*m', '', text)  # curl -i prints header names in bold
    lines = text.split('\n')
    while lines and lines[-1].strip() in ('', '$', '$ exit', 'exit'):
        lines.pop()
    return '\n'.join(lines).lstrip('\n').expandtabs()


if __name__ == '__main__':
    for name in sys.argv[1:] or SESSIONS:
        print(f'=== {name}')
        print(run(name))
