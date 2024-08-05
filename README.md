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
- 📊 представляет анализ и статистику<br>
- 🎨 символизирует форматирование и визуальное представление<br>
- 📁 обозначает работу с директориями<br>
- 📝 ассоциируется с текстовыми файлами и анализом<br>
- 🎵 представляет аудио и видео файлы<br>
- 📄 символизирует PDF-документы

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
`inf document.txt`

2. Get information about a video file:<br>
`inf video.mp4`

3. Analyze a directory:<br>
`inf /path/to/directory`

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
