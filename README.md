# i86emu

**i86emu** is an emulator for the **Intel 8086** processor, aimed at exploring the fundamentals of the classic x86 architecture.  
The project focuses on **clarity of implementation and core concepts**, rather than full accuracy or performance, making it a solid base for studying or extending CPU emulation.

It provides insights into:

- 8086 instructions (ALU operations, jumps, stack operations, etc.)
- General-purpose, segment, and status registers
- Opcode decoding and the ModR/M byte
- Flag behavior and internal CPU logic
- The structure of a CPU emulator

## 🎯 Goals

- Provide a **readable, well-commented implementation**
- Serve as a **reference** for personal or academic projects

## 🔮 Future Plans

- [ ] Automated documentation with Doxygen
- [ ] Unit tests with explanations of instruction behavior and flag effects
- [ ] Additional features to enhance completeness and usability

## 🚀 Status

The project is **under development**. The code is still being cleaned, reviewed and documented, and it is **not fully functional yet**. Improvements will be made continuously.

## 🛠️ Usage

### 🔹 Building

The project includes build scripts for both Windows and Linux.

#### Windows
```bash
build.bat
```

#### Linux

```bash
./build.sh
```

Requires **zenity** for dialog prompts

```bash
sudo apt install zenity   # Debian/Ubuntu
sudo dnf install zenity   # Fedora
```

### 🔹 Running

After building, the executable will be located in the build/bin/Release directory.
Run it with:

```bash
./build/bin/Release/i86emu
```

## 📖 References

- **Intel 8086 Family User’s Manual** – Intel Corporation
- **The 8086 Book** – Russell Rector & George Alexy
- [Intel 64 and IA-32 Architectures Software Developer’s Manual](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html) – Intel Corporation
