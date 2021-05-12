# elf-extractor
This small program aims to extract the code part of any Elf (x86-64) files as a valid C-string.

*disclaimer : early stage project, it only handles well-formed ELF64 bits for little endian processors*

## Usage

```
./install.sh
elf-extractor filename
```

Modify `install.sh` variables to custom the installation : 
- `install_dir` : The location where you want the binary, This location will then be added to your `$PATH` which means you can actually install several scripts you use there.
- `bin_name` : The name you want for the binary
- `SHELL` : Your shell path (important to add the binary path to your `$PATH`)

