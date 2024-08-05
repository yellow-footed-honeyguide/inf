from setuptools import setup

# Read the contents of README.md for the long description
with open("README.md", "r", encoding="utf-8") as fh:
    long_description = fh.read()

setup(
    # Package name - should be unique on PyPI
    name="inf",
    # Version following Semantic Versioning (SemVer)
    version="1.0.0",
    # Author details
    author="Sergey Veneckiy",
    author_email="s.venetsky@gmail.com",
    # Short description (will appear on PyPI)
    description="A utility for providing detailed file information",
    # Detailed description (will appear on PyPI page)
    long_description=long_description,
    long_description_content_type="text/markdown",
    # Project's main homepage
    url="https://github.com/yellow-footed-honeyguide/inf",
    # Modules to include in the package
    py_modules=["inf"],
    # Provide package metadata to categorize your package
    classifiers=[
        "Development Status :: 3 - Alpha",
        "Intended Audience :: Developers",
        "License :: OSI Approved :: MIT License",
        "Operating System :: POSIX",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "Programming Language :: Python :: 3.12",
    ],
    # Specify the Python versions you support here
    python_requires=">=3.8",
    # List additional groups of dependencies here
    install_requires=[
        "rich",
    ],
    # To provide executable scripts, use entry points
    entry_points={
        "console_scripts": [
            # 'script_name=module_name:function_name'
            "inf=inf:main",
        ],
    },
)
