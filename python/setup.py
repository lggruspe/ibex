import setuptools

with open("../README.md", "r") as f:
    long_description = f.read()

setuptools.setup(
    name="rnd",
    version="0.2a1",
    author="Levi Gruspe",
    author_email="mail.levig@gmail.com",
    description="Python bindings for libcrnd",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/lggruspe/rnd",
    packages=["rnd"],
    install_requires=["redblack"],
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: POSIX :: Linux"
    ])
