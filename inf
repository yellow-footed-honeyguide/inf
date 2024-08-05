#!/usr/bin/env python3

"""
File Information Utility

This script provides detailed information about a file or directory in a well-formatted table.

Author: Sergey Veneckiy
Email: s.venetsky@gmail.com
GitHub: https://github.com/yellow-footed-honeyguide/inf

Developed on Python 3.12
Compatible with Python 3.8+ and Unix-like systems

System dependencies:
- file
- du
- pdfinfo
- ffprobe

Python dependencies:
- rich

Usage: python3 file_info.py [OPTIONS] <file_or_directory_path>
"""

import os
import sys
import subprocess
import argparse
import mimetypes
from typing import List, Dict
from rich.console import Console
from rich.table import Table
from datetime import timedelta

__version__ = "1.0.0"

def run_command(command: List[str]) -> str:
    """Run a shell command and return its output."""
    try:
        return subprocess.check_output(command, universal_newlines=True, stderr=subprocess.DEVNULL).strip()
    except subprocess.CalledProcessError:
        return "Error: Command failed"

def get_file_type(path: str) -> str:
    """Get the file type using the 'file' command."""
    output = run_command(['file', path])
    return output.replace(f"{path}: ", "")

def get_file_size(path: str) -> str:
    """Get the file or directory size."""
    if os.path.isdir(path):
        return run_command(['du', '-sh', path]).split()[0]
    else:
        return run_command(['ls', '-lh', path]).split()[4]

def format_duration(seconds: float) -> str:
    """Format duration in seconds to HH:MM:SS.mmm format."""
    td = timedelta(seconds=seconds)
    hours, remainder = divmod(td.seconds, 3600)
    minutes, seconds = divmod(remainder, 60)
    return f"{hours:02d}:{minutes:02d}:{seconds:02d}.{td.microseconds // 1000:03d}"

def parse_audio_info(info: str) -> Dict[str, str]:
    """Parse audio file information into a dictionary."""
    parts = info.split(',')
    result = {}
    for part in parts:
        part = part.strip()
        if ':' in part:
            key, value = part.split(':', 1)
        else:
            key, value = part, ''
        result[key.strip()] = value.strip()
    return result

class FileInfo:
    """Class to handle file information gathering and display."""

    def __init__(self, path: str):
        self.path = path
        self.console = Console()
        self.table = Table(title=f"File Information: {path}")
        self.file_type = get_file_type(path)
        self.file_size = get_file_size(path)

    def add_row(self, key: str, value: str):
        """Add a row to the table."""
        self.table.add_row(key, value)

    def add_basic_info(self):
        """Add basic file information to the table."""
        self.table.add_column("Attribute", style="cyan", no_wrap=True)
        self.table.add_column("Value", style="magenta")
        self.add_row("File name", self.path)
        self.add_row("File type", self.file_type)
        self.add_row("File size", self.file_size)

    def add_pdf_info(self):
        """Add PDF-specific information."""
        pdf_info = run_command(['pdfinfo', self.path])
        for line in pdf_info.split('\n'):
            key, value = line.split(':', 1)
            self.add_row(key.strip(), value.strip())

    def add_text_file_info(self):
        """Add text file-specific information."""
        with open(self.path, 'rb') as f:
            lines = sum(1 for _ in f)
        self.add_row("Number of Lines", str(lines))

        wc_output = run_command(['wc', '-w', '-m', self.path])
        word_count, char_count, _ = wc_output.split()
    
        self.add_row("Word Count", word_count)
        self.add_row("Character Count", char_count)

    def add_video_info(self):
        """Add video file-specific information."""
        duration = float(run_command(['ffprobe', '-v', 'error', '-show_entries', 'format=duration', '-of', 'default=noprint_wrappers=1:nokey=1', self.path]))
        formatted_duration = format_duration(duration)
        self.add_row("Duration", formatted_duration)

    def add_executable_info(self):
        """Add executable file-specific information."""
        info = get_file_type(self.path)
        for item in info.split(','):
            key, value = item.split(':', 1) if ':' in item else (item.strip(), '')
            self.add_row(key.strip(), value.strip())

    def add_audio_info(self):
        """Add audio file-specific information."""
        info = get_file_type(self.path)
        parsed_info = parse_audio_info(info)
        
        if 'RIFF' in parsed_info:
            self.add_row("Audio Format", "RIFF (WAV)")
        elif 'Audio file' in parsed_info:
            self.add_row("Audio Format", parsed_info.get('Audio file', 'Unknown'))
        
        if 'bit' in parsed_info:
            self.add_row("Bit Depth", parsed_info['bit'])
        
        if 'Hz' in parsed_info:
            self.add_row("Sample Rate", parsed_info['Hz'])
        
        if 'stereo' in info.lower():
            self.add_row("Channels", "Stereo")
        elif 'mono' in info.lower():
            self.add_row("Channels", "Mono")
        
        for key, value in parsed_info.items():
            if key not in ['RIFF', 'Audio file', 'bit', 'Hz'] and value:
                self.add_row(key, value)

    def process(self):
        """Process the file and gather relevant information."""
        self.add_basic_info()

        mime_type, _ = mimetypes.guess_type(self.path)

        if mime_type and mime_type.startswith('text') or 'text' in self.file_type.lower():
            self.add_text_file_info()
        
        if self.path.lower().endswith(('.mkv', '.mp4')):
            self.add_video_info()
        elif self.path.lower().endswith('.pdf'):
            self.add_pdf_info()
        elif 'executable' in self.file_type.lower():
            self.add_executable_info()
        elif 'audio' in self.file_type.lower():
            self.add_audio_info()
        elif os.path.isdir(self.path):
            pass  # Basic info is sufficient for directories

    def display(self):
        """Display the gathered information in a table format."""
        self.console.print(self.table)

def main():
    """Main function to run the script."""
    parser = argparse.ArgumentParser(description="File Information Utility")
    parser.add_argument("path", nargs="?", help="Path to the file or directory")
    parser.add_argument("-v", "--version", action="version", version=f"%(prog)s {__version__}")
    args = parser.parse_args()

    if not args.path:
        parser.print_help()
        sys.exit(1)

    file_path = args.path
    if not os.path.exists(file_path):
        print(f"Error: The file or directory '{file_path}' does not exist.")
        sys.exit(1)

    file_info = FileInfo(file_path)
    file_info.process()
    file_info.display()

if __name__ == "__main__":
    main()
