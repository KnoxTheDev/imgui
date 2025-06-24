#!/usr/bin/env python3
import os
import subprocess
import sys

VENV_DIR = os.path.join(os.getcwd(), "venv")
TARGET_DIR = os.path.join(os.getcwd(), "xhook")

def create_venv():
    if not os.path.isdir(VENV_DIR):
        print("🔧 Creating virtual environment...")
        subprocess.check_call([sys.executable, "-m", "venv", VENV_DIR])
    else:
        print("✅ Virtual environment already exists")

def install_dependencies():
    pip = os.path.join(VENV_DIR, "bin", "pip") if os.name != "nt" else os.path.join(VENV_DIR, "Scripts", "pip.exe")
    print("📦 Installing dependencies...")
    subprocess.check_call([pip, "install", "requests"])

def run_downloader():
    python = os.path.join(VENV_DIR, "bin", "python") if os.name != "nt" else os.path.join(VENV_DIR, "Scripts", "python.exe")
    code = f'''
import os, requests, sys

url = "https://api.github.com/repos/iqiyi/xHook/contents/libxhook/jni"
os.makedirs("{TARGET_DIR}", exist_ok=True)
r = requests.get(url)
if r.status_code != 200:
    print("❌ GitHub API failed", file=sys.stderr)
    sys.exit(1)

for f in r.json():
    if f["name"].endswith((".c", ".h")):
        raw = requests.get(f["download_url"])
        if raw.status_code == 200:
            with open(os.path.join("{TARGET_DIR}", f["name"]), "wb") as out:
                out.write(raw.content)
            print("✅", f["name"], "downloaded")
        else:
            print("❌ Failed:", f["name"])
'''

    print("⬇️ Downloading xhook .c/.h files...")
    subprocess.check_call([python, "-c", code])

if __name__ == "__main__":
    create_venv()
    install_dependencies()
    run_downloader()
