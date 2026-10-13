"""Exercise the lecture reverse implementation under ASan and UBSan."""
import pathlib
import random
import subprocess
import tempfile

ROOT = pathlib.Path(__file__).resolve().parent


def expected_output(data, chars):
    lines = data.split(b"\n")
    if data.endswith(b"\n") or not data:
        lines = lines[:-1]
    return b"".join(
        (line[::-1] if chars else line) + b"\n" for line in reversed(lines)
    )


def verify():
    with tempfile.TemporaryDirectory(prefix="reverse-check-") as temp:
        directory = pathlib.Path(temp)
        binary = directory / "reverse-san"
        subprocess.run([
            "gcc", "-std=c17", "-Wall", "-Wextra", "-Werror",
            "-fsanitize=address,undefined", "-g", str(ROOT / "reverse.c"),
            "-o", str(binary),
        ], check=True)

        def run(data, chars=False, file=False, ok=True):
            args = [str(binary)] + (["-c"] if chars else [])
            if file:
                (directory / "input").write_bytes(data)
                args.append(str(directory / "input"))
            result = subprocess.run(
                args, input=None if file else data, cwd=directory,
                stdout=subprocess.PIPE, stderr=subprocess.PIPE,
            )
            assert (result.returncode == 0) == ok, result.stderr
            assert b"Sanitizer" not in result.stderr, result.stderr
            if ok:
                assert result.stdout == expected_output(data, chars), (
                    data[:80], result.stdout[:80], expected_output(data, chars)[:80]
                )

        cases = [
            b"", b"one", b"one\ntwo", b"a\n\nb\n", b"\n",
            b"a" * 4095 + b"\n", b"a" * 4096,
            b"a" * 4000 + b"\n" + b"b" * 200 + b"\n",
            b"x\n" * 100000, "άλφα\nβήτα\n".encode(),
        ]
        count = 0
        for data in cases:
            for file in [False, True]:
                for chars in [False, True]:
                    # With -c, deliberately check bytes, not Unicode reversal.
                    run(data, chars=chars, file=file)
                    count += 1
        for data in [b"x" * 4097, b"x\n" * 100001]:
            run(data, ok=False)
            count += 1
        generator = random.Random(13)
        for _ in range(30):
            lines = [
                bytes(generator.choices(b"abcde ", k=generator.randrange(0, 40)))
                for _ in range(generator.randrange(1, 30))
            ]
            data = b"\n".join(lines) + (b"\n" if generator.randrange(2) else b"")
            run(data, chars=bool(generator.randrange(2)))
            count += 1
        for arg in ["--bad-option", "missing-file"]:
            result = subprocess.run(
                [str(binary), arg], cwd=directory,
                stdout=subprocess.PIPE, stderr=subprocess.PIPE,
            )
            assert result.returncode != 0 and b"Sanitizer" not in result.stderr
            count += 1
        print(f"{count} reverse checks passed (ASan + UBSan).")


if __name__ == "__main__":
    verify()
