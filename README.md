# File Information Utility

## Overview

- 📊 The File Information Utility is a powerful command-line tool written in C that provides:
- 🔍 Detailed information about files
- 📊 Comprehensive analysis of various file types
- 📋 Data presentation in an easy-to-read format

🚀 Whether you're a developer, system administrator, or power user, this tool streamlines the process of gathering and visualizing file metadata, making your workflow more efficient and informative.


## Features

- 📊 Basic file information (size, permissions, last modified date)
- 🔠 MIME type detection
- 📝 Text file analysis (line, word, and character count)
- 🖼️ Image file information (dimensions, color space)
- 🎥 Video file duration
- 📄 PDF document details
- 📦 Archive file information (file count, total uncompressed size)

## Requirements

### Supported Operating Systems

- Linux
- Unix-like systems

### System Dependencies

- libmagic
- ImageMagick (for image analysis)
- FFmpeg (for video analysis)
- Poppler (for PDF analysis)
- p7zip (for archive analysis)

## Installation

### Building from Source

Clone the repository:
```
git clone https://github.com/yellow-footed-honeyguide/inf.git
cd inf
mkdir build
cd build
meson ..
ninja
```
or install utility system-wide:
```
sudo ninja install
```

## Usage
inf [OPTIONS] <file_path>

### Options

- `-h`, `--help`   : Show help message and exit
- `-v`, `--version`: Show program's version number and exit

## Examples

1. Analyze a text file: `inf ./docs/document.txt`
2. Get information about a video file: `inf video.mp4`
3. Analyze a PDF document: `inf document.pdf`


## Contributing

Contributions to the File Information Utility are welcome! Please feel free to submit a Pull Request.


## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.


## Author

**Sergey Veneckiy**
- Email: s.venetsky@gmail.com
- GitHub: [@yellow-footed-honeyguide](https://github.com/yellow-footed-honeyguide)

## Acknowledgments

- This project uses various open-source libraries and tools, including libmagic, ImageMagick, FFmpeg, Poppler, and p7zip.
