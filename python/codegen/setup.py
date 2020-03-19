import setuptools

with open("README.md", "r") as f:
    long_description = f.read()

setuptools.setup(
    name="ibex-codegen",
    version="0.1a0",
    author="Levi Gruspe",
    author_email="mail.levig@gmail.com",
    description="Template-based code generator",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/lggruspe/ibex",
    modules=["codegen.py"],
    install_requires=["jinja2"],
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
    ])
