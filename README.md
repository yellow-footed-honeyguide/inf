# File Information Utility

## Overview
The File Information Utility is a powerful command-line tool designed to provide detailed information about files and directories. It offers a comprehensive analysis of various file types, presenting the data in a well-formatted, easy-to-read table.

## Features
- Detailed file analysis for various file types (text, audio, video, PDF, executables)
- Well-formatted output using rich tables
- Support for directory analysis
- Comprehensive text file analysis (line count, word count, character count)
- Audio and video file metadata extraction
- PDF file information retrieval
- Executable file details

## Requirements
### Supported Operating Systems
- Linux
- macOS
- Unix-like systems

### System Dependencies
- file
- du
- pdfinfo
- ffprobe

### Python Version
- Developed on Python 3.12
- Compatible with Python 3.8+

### Python Dependencies
- rich

## Installation
1. Clone the repository:
git clone https://github.com/yellow-footed-honeyguide/inf.git
Copy2. Navigate to the project directory:
cd inf
Copy3. Install the required Python package:
pip install rich

## Usage
python3 file_info.py [OPTIONS] <file_or_directory_path>

### Options
- `-h`, `--help`: Show help message and exit
- `-v`, `--version`: Show program's version number and exit

## Examples
1. Analyze a text file:
python3 file_info.py document.txt
Copy2. Get information about a video file:
python3 file_info.py movie.mp4
Copy3. Analyze a directory:
python3 file_info.py /path/to/directory
Copy
## Contributing
Contributions to the File Information Utility are welcome! Please feel free to submit a Pull Request.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Author
- **Sergey Veneckiy**
- Email: s.venetsky@gmail.com
- GitHub: [@yellow-footed-honeyguide](https://github.com/yellow-footed-honeyguide)

## Acknowledgments
- This project uses the [rich](https://github.com/willmcgugan/rich) 
