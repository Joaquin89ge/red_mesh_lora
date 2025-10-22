# -- Path setup --------------------------------------------------------------
import os
import sys
sys.path.insert(0, os.path.abspath('..'))

# -- Project information -----------------------------------------------------
project = 'Sistema Red Mesh Agrícola'
copyright = '2025, Equipo de Desarrollo Agrícola'
author = 'Equipo de Desarrollo Agrícola'
release = '1.0.0'
version = '1.0.0'

# -- General configuration ---------------------------------------------------
extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.doctest',
    'sphinx.ext.intersphinx',
    'sphinx.ext.todo',
    'sphinx.ext.coverage',
    'sphinx.ext.mathjax',
    'sphinx.ext.ifconfig',
    'sphinx.ext.viewcode',
    'sphinx.ext.githubpages',
    'sphinx_rtd_theme',
    'sphinxcontrib.mermaid',
]

templates_path = ['_templates']
language = 'es'
master_doc = 'index'
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

# -- Options for HTML output -------------------------------------------------
html_theme = 'sphinx_rtd_theme'
html_theme_options = {
    'navigation_depth': 4,
    'collapse_navigation': False,
    'sticky_navigation': True,
    'includehidden': True,
    'titles_only': False,
    'logo_only': False,
    'display_version': True,
    'prev_next_buttons_location': 'bottom',
    'style_external_links': True,
    'style_nav_header_background': '#2980B9',
}
html_static_path = ['_static']
html_css_files = ['custom.css']

# -- Options for autodoc -----------------------------------------------------
autodoc_typehints = 'description'
autodoc_inherit_docstrings = False

# -- Options for todo extension ----------------------------------------------
todo_include_todos = True

# -- Options for intersphinx extension ---------------------------------------
intersphinx_mapping = {'python': ('https://docs.python.org/3/', None)}

# -- Custom CSS for better styling -------------------------------------------


def setup(app):
    app.add_css_file('custom.css')
