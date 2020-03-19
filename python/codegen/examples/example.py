from os.path import abspath, dirname, join
import sys

sys.path.append(abspath(join(dirname(__file__), "..")))
import codegen

doc = codegen.render("template.html", context={
    "title": "Title",
    "body": "Hello, world!",
})
print(doc)
