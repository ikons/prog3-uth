#!/usr/bin/env python3
"""Replay a terminal session in an interactive bash on a pseudo-terminal.

Job-control messages ([1] 4698, Done), Control-D at the end of typed input and
the window size seen by ioctl() need a real terminal, as in the lecture.
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
    'fifo': 'rm -f myPipe',
    'fifoprog': 'rm -f aPipe',
    'monitor': 'rm -rf mon && mkdir -p mon/tmp && cp monitor monitor.c mon/'
               ' && printf "hi there\\n" > mon/tmp/b'
               ' && touch -d "2026-09-28 10:15:00" mon/tmp/b',
    'filetype': 'rm -f aFifo link.c && mkfifo aFifo && ln -s monitor.c link.c',
    'chown': 'rm -f test.txt && printf "hi\\n" > test.txt && chmod 644 test.txt',
    'fcntl': 'printf "here are the contents of\\nthe original file.\\n" > test.txt',
    'link': 'rm -f original.txt another.txt && printf "this is a file.\\n" > original.txt',
    'truncate': 'printf "short\\n" > file1.txt'
                ' && printf "long file with lots of letters\\n" > file2.txt',
    'inotify': 'rm -rf watch && mkdir watch',
}
DIRS = {'monitor': 'mon'}
SIZES = {'ioctl': (24, 80)}

SESSIONS = {
    'talk': [(0.3, './talk\n')],
    'connect': [(0.3, 'ls *.c | head -4\n'), (0.5, './connect ls wc\n'),
                (0.5, 'ls | wc\n')],
    'eof': [(0.3, './eof\n'), (0.5, 'timeout 2 ./eof keep; echo $?\n'), (2.5, '')],
    'sigpipe': [(0.3, './sigpipe; echo $?\n'), (0.5, './sigpipe ignore; echo $?\n')],
    'yes': [(0.3, 'yes | head -n 2; echo "${PIPESTATUS[@]}"\n')],
    'fifo': [(0.3, 'mknod myPipe p\n'), (0.3, 'chmod ug+rw myPipe\n'),
             (0.3, 'ls -l myPipe\n'), (0.3, 'echo hello > myPipe &\n'),
             (0.5, 'cat < myPipe\n'), (0.5, 'rm myPipe\n')],
    'fifoprog': [(0.3, './reader & ./writer & ./writer &\n'), (10.5, '\n')],
    'monitor': [(0.3, 'ls\n'), (0.3, 'ls tmp\n'),
                (0.3, './monitor -t 1 tmp myFile.txt > monitor.out 2>&1 &\n'),
                (1.5, 'cat > tmp/a\n'), (0.3, 'hi there\n'), (0.3, CTRL_D),
                (1.5, 'cat > myFile.txt\n'), (0.3, 'hi there\n'), (0.3, CTRL_D),
                (1.5, 'cat >> myFile.txt\n'), (0.3, 'hi again\n'), (0.3, CTRL_D),
                (1.5, 'rm tmp/a\n'), (1.5, 'jobs\n'), (0.3, 'kill %1\n'),
                (0.5, 'cat monitor.out\n')],
    'stat': [(0.3, 'stat monitor.c\n')],
    'filetype': [(0.3, './filetype monitor.c link.c . aFifo /dev/null\n')],
    'chown': [(0.3, 'ls -l test.txt\n'), (0.3, './mychown users\n'),
              (0.3, 'ls -l test.txt\n'), (0.3, './mychown root\n'),
              (0.3, './mychmod\n'), (0.3, 'ls -l test.txt\n')],
    'dup': [(0.3, './mydup\n'), (0.3, 'cat test.txt\n')],
    'fcntl': [(0.3, 'cat test.txt\n'), (0.3, './myfcntl\n'), (0.3, 'cat test.txt\n')],
    'ioctl': [(0.3, './winsize\n'), (0.3, './winsize | cat\n'), (0.3, 'stty size\n')],
    'link': [(0.3, 'ls -li original.txt another.txt\n'), (0.3, './mylink\n'),
             (0.3, 'ls -li original.txt another.txt\n'), (0.3, 'cat >> another.txt\n'),
             (0.3, 'hi\n'), (0.3, CTRL_D), (0.3, 'ls -li original.txt another.txt\n'),
             (0.3, 'rm original.txt\n'), (0.3, 'ls -li original.txt another.txt\n'),
             (0.3, 'cat another.txt\n')],
    'mknod': [(0.3, 'ls -l /dev/null /dev/tty\n'), (0.3, 'mknod mynull c 1 3\n')],
    'truncate': [(0.3, 'ls -l file*.txt\n'), (0.3, './truncate\n'),
                 (0.3, 'ls -l file*.txt\n'), (0.3, 'od -c file1.txt\n'),
                 (0.3, 'cat file2.txt\n')],
    'pipesize': [(0.3, './pipesize\n')],
    'procfd': [(0.3, 'sleep 100 | sleep 100 &\n'),
               (0.5, 'ls -l /proc/$(jobs -p)/fd /proc/$!/fd | grep -E "^/|pipe"\n'),
               (0.5, 'kill %1\n')],
    'strace': [(0.3, "strace -f -qq -e signal=none -e trace=pipe2,dup2,execve bash -c 'ls | wc -l'\n"),
               (1.5, '')],
    'inotify': [(0.3, 'inotifywait -m -q -e create,close_write,moved_to,delete watch &\n'),
                (0.8, 'echo hi > watch/a; sleep 0.3\n'), (0.6, 'mv watch/a watch/b; sleep 0.3\n'),
                (0.6, 'rm watch/b; sleep 0.3\n'), (0.6, 'kill %1\n')],
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
