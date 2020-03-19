from os.path import abspath, dirname, join
import jinja2

def render(entrypoint, *, context, directory=None):
    paths = [abspath(join(dirname(__file__), "templates"))]
    if directory:
        paths = [directory, *paths]
    loader = jinja2.FileSystemLoader(paths)
    env = jinja2.Environment(loader=loader)
    template = env.get_template(entrypoint)
    return template.render(**context)
