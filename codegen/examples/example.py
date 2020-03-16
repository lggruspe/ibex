import codegen

doc = codegen.render("template.html", context={
    "title": "Title",
    "body": "Hello, world!",
})
print(doc)
