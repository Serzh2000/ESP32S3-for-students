"""Install the exact web-panel library sources into an isolated directory.

Usage: python tools/install_web_libraries.py --output /path/to/libraries
Then pass that directory to arduino-cli compile --libraries.
Existing directories are reused only when clean and at the locked commit.
"""
import argparse
import json
from pathlib import Path
import subprocess


def git(*args, capture=False):
    return subprocess.run(
        ["git", *map(str, args)], check=True, text=True,
        stdout=subprocess.PIPE if capture else None,
    ).stdout


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()
    root = Path(__file__).resolve().parent.parent
    lock = json.loads(
        (root / "examples/web-panels/dependencies.lock.json").read_text("utf-8")
    )
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    for library in lock["libraries"]:
        destination = output / library["directory"]
        if destination.exists():
            head = git("-C", destination, "rev-parse", "HEAD", capture=True).strip()
            dirty = git("-C", destination, "status", "--porcelain", capture=True)
            if head != library["commit"] or dirty:
                raise RuntimeError(
                    f"Refusing to modify existing library: {destination}. "
                    "Choose a fresh output directory."
                )
        else:
            destination.mkdir()
            git("init", destination)
            git("-C", destination, "remote", "add", "origin", library["url"])
            git("-C", destination, "fetch", "--depth", "1", "origin", library["commit"])
            git("-C", destination, "checkout", "--detach", "FETCH_HEAD")
        print(f"{library['name']} {library['version']}: {library['commit']}")


if __name__ == "__main__":
    main()
