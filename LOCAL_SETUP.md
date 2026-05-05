# Local Setup (Windows)

This repo contains:
- The Jatti C compiler + bytecode VM (`c/`)
- A VS Code extension (publishable scaffold in `jatti-vscode-extension-c/`)

## Requirements

- Windows 10/11
- Git (optional, but recommended)
- Visual Studio 2022 Build Tools (MSVC)

## 1) Get the code

```powershell
git clone https://github.com/s-angad/jatti
cd Punjabi-Language-Jatti-
```

If you already have the folder, just `cd` into it.

## 2) Create and activate a virtual environment

No setup is required for the C VM.

## 3) Run a Jatti file (CLI)

Create `hello.jatti`:

```jatti
sun_we
    chilla_we "Hello Jatti!"
ja_we
```

Run:

```powershell
   jatti run hello.jatti
```
