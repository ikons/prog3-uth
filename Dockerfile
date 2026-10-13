# Το περιβάλλον των διαλέξεων: GCC 15.3 σε Debian 13 (trixie), χρήστης student.
#
#   docker build -t prog3 .
#   docker run --rm -it --init --hostname unix -v "$PWD":/home/student/examples prog3
#
# Σε Linux, για να γράφουν τα αρχεία στον κατάλογο του host με τον δικό σας
# χρήστη: docker build --build-arg STUDENT_UID="$(id -u)" -t prog3 .

# Το tag λέει την έκδοση· το digest κρατά το image ίδιο σε κάθε build.
FROM gcc:15.3.0@sha256:ead103e6d03b69232962d467f3520c3f70b6718c69ff71efcc08efe9011fadb6

LABEL org.opencontainers.image.title="prog3" \
      org.opencontainers.image.description="Προγραμματισμός ΙΙΙ: GCC 15.3, gdb, strace, valgrind και τα εργαλεία των διαλέξεων σε Debian 13"

ARG DEBIAN_FRONTEND=noninteractive

# Το gcc, το make, το git και τα binutils (gprof) τα έχει ήδη η βάση.
# - αποσφαλμάτωση: gdb, strace, ltrace, valgrind
# - κελύφη και σκριπτ: ksh (κεφ. 4), bc (cardstats.sh), vim, nano
# - διεργασίες, αρχεία, δίκτυο: procps (ps), psmisc (pstree, killall), lsof,
#   inotify-tools (13.4), iproute2 (ss), netcat-openbsd (nc), curl (13.5)
# - python3: τα session.py και το tasks.py
# - man-db, manpages, manpages-dev: man 2 open, man 3 printf
# Χωρίς εκδόσεις στα πακέτα (DL3008): η βάση είναι καρφωμένη με digest, και στο
# Debian stable οι εκδόσεις αλλάζουν μόνο με διορθώσεις ασφαλείας.
# hadolint ignore=DL3008
RUN apt-get update \
 && apt-get install -y --no-install-recommends \
        bc \
        curl \
        gdb \
        inotify-tools \
        iproute2 \
        ksh \
        less \
        lsof \
        ltrace \
        man-db \
        manpages \
        manpages-dev \
        nano \
        netcat-openbsd \
        procps \
        psmisc \
        python3 \
        strace \
        valgrind \
        vim \
 && rm -rf /var/lib/apt/lists/*

# Χρήστης χωρίς δικαιώματα root, και μέλος της ομάδας users για το mychown (13.4).
ARG STUDENT_UID=1000
RUN useradd --create-home --uid "${STUDENT_UID}" --groups users --shell /bin/bash student

ENV LANG=C.UTF-8
USER ${STUDENT_UID}
WORKDIR /home/student/examples
CMD ["bash"]
