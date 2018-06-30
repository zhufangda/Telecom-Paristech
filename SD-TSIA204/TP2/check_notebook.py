#!/usr/bin/env python

"""Linter for notebooks."""
import sys
import glob
import json
from os import path
import unittest

# Authors: Bob Emploi modified by Alex Gramfort

# License: GNU GPL v3


__version__ = 0.1


class NotebookLintCase(unittest.TestCase):
    """Linter for notebooks."""
    NOTEBOOKS = None

    def test_code_cell_syntax(self):
        """Check some specific features are not used in code cells."""
        for file_name, notebook in self.NOTEBOOKS:
            coding_cells = [
                c for c in notebook.get('cells', [])
                if c.get('cell_type') == 'code']
            for coding_cell_number, coding_cell in enumerate(coding_cells):
                for line in coding_cell.get('source', []):
                    # Forbid imports except in the first cell.
                    msg = (
                        'Imports should be only in first coding cell, but in '
                        'file "{}", coding cell '
                        '{:d}'.format(file_name, coding_cell_number))
                    if coding_cell_number:
                        self.assertNotRegex(line, '^import ', msg=msg)
                        self.assertNotRegex(line, '^from .* import ', msg=msg)

                    # Forbid assignment to _.
                    self.assertNotRegex(
                        line, r'^_\s*=',
                        msg='Do not assign to the _ variable, use a '
                            'trailing ; instead.')

                    # Avoid config options that are already set by default.
                    self.assertNotEqual(
                        '%matplotlib inline', line.strip(),
                        msg='No need to set "%matplotlib inline", it\'s '
                            'already in the config.')
                    self.assertNotEqual(
                        "%config InlineBackend.figure_format = 'retina'",
                        line.strip(), msg="No need to set this config, it's "
                                          "already in the default config "
                                          "file.")

    def test_at_least_one_cell(self):
        """Check that each notebook contains at least one cell."""
        for file_name, notebook in self.NOTEBOOKS:
            msg = '{} has no cells'.format(file_name)
            self.assertGreater(len(notebook.get('cells', [])), 1, msg=msg)

    def test_first_cell_contains_author(self):
        """Check that the first cell is a markdown cell with the author."""
        for file_name, notebook in self.NOTEBOOKS:
            cells = notebook.get('cells', [])
            if not cells:
                continue
            first_cell = cells[0]
            msg = (
                'First cell of {} should be a markdown cell containing the '
                "original author's name".format(file_name))
            self.assertEqual('markdown', first_cell.get('cell_type'), msg=msg)
            author_found = False
            for line in first_cell.get('source', []):
                if line.startswith('Author: ') or line.startswith('Authors: '):
                    author_found = True
                    break
            self.assertTrue(author_found, msg=msg)

    def test_python3(self):
        """Check that the notebooks are using Python 3 kernel only."""
        for file_name, notebook in self.NOTEBOOKS:
            kernel = notebook['metadata']['kernelspec'].get('name')
            msg = (
                'The notebook {} is using kernel {} instead of python3'
                .format(file_name, kernel))
            self.assertEqual('python3', kernel, msg=msg)

    def test_no_spaces_in_filenames(self):
        """Check that the notebooks names use underscores, not blank spaces."""
        for file_name, unused_notebook in self.NOTEBOOKS:
            self.assertNotIn(
                ' ', path.basename(file_name),
                msg='Use underscore in filename {}'.format(file_name))

    def test_filenames_in_lowercase(self):
        """Check that the notebooks names only use lowercases."""
        for file_name, unused_notebook in self.NOTEBOOKS:
            basename = path.basename(file_name)
            self.assertEqual(
                basename.lower(), basename,
                msg='Use lowercase only in filename "{}"'.format(file_name))

    def test_clean_execution(self):
        """Check that all code cells have been run once in the right order."""
        for file_name, notebook in self.NOTEBOOKS:
            cells = notebook.get('cells', [])
            code_cells = [c for c in cells if c.get('cell_type') == 'code']
            for index, cell in enumerate(code_cells):
                self.assertTrue(
                    cell.get('source'),
                    msg='There is an empty code cell in '
                        'notebook {}'.format(file_name))
                self.assertEqual(
                    index + 1, cell.get('execution_count'),
                    msg='The code cells in notebook {} have not been executed '
                    'in the right order. Run "Kernel > Restart & run all" '
                    'then save the notebook.'.format(file_name))


def get_notebooks(argv):
    files = []
    for p in argv[1:]:
        filenames = sorted(glob.glob(p))
        for fname in filenames:
            if fname.endswith('ipynb'):
                files.append(fname)

    notebooks = []
    for filename in files:
        with open(filename) as notebook_file:
            try:
                notebooks.append((filename, json.load(notebook_file)))
            except json.decoder.JSONDecodeError:
                raise ValueError('"%s" is not a valid JSON file.' % filename)
    return notebooks


if __name__ == "__main__":
    argv = []
    while len(sys.argv) > 1:
        argv.append(sys.argv.pop())
    notebooks = get_notebooks(argv)
    NotebookLintCase.NOTEBOOKS = notebooks
    unittest.main()
