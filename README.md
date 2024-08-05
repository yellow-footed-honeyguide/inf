# File Information Utility

## Overview

📊 The File Information Utility is a powerful command-line tool that provides:

- 🔍 Detailed information about files and directories
- 📊 Comprehensive analysis of various file types
- 📋 Data presentation in well-formatted, easy-to-read tables

🚀 Whether you're a developer, system administrator, or power user, this tool 
   streamlines the process of gathering and visualizing file metadata, making 
   your workflow more efficient and informative.

## Features
- 📊 Represents analysis and statistics<br>
- 🎨 Symbolizes formatting and visual presentation<br>
- 📁 Denotes work with directories<br>
- 📝 Associated with text files and analysis<br>
- 🎵 Represents audio and video files<br>
- 📄 Symbolizes PDF documents

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
`
sudo pip install git+https://github.com/yellow-footed-honeyguide/inf.git
`

## Usage
python3 file_info.py [OPTIONS] <file_or_directory_path>

### Options
- `-h`, `--help`: Show help message and exit
- `-v`, `--version`: Show program's version number and exit

## Examples
1. Analyze a text file:<br>
`❯ inf ./docs/document.txt`

2. Get information about a video file:<br>
`❯ inf video.mp4`

3. Analyze a directory:<br>
`❯ inf /path/to/directory`

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
