import setuptools

with open("README.md", "r") as fh:
    long_description = fh.read()

setuptools.setup(
    name="hodl",
    version="1.0.0",
    author="Ayush Tambde",
    author_email="tambdeayush@gmail.com",
    description="Quantum Oracle Programming Language",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/at2005/HODL",
    packages=setuptools.find_packages(),
    classifiers=(
        "Programming Language :: C++ :: 11",
        "License :: OSI Approved :: MIT License",
        "Operating System :: Linux :: MacOS",
    ),
)
