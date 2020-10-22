"""Generate a contents table of a jupyter notebook.
This scripts outputs to standard output, can simply copy and paste it in a markdown cell.

Example of usage (get a numbered list):
    $ python3 gen_contents.py MyNotebook.ipynb

To get an un-numbered list, use optional parameter `-b` or `--bullet`.
"""

import argparse
import json
import re


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('file', help='.ipynb file')
    parser.add_argument('-b', '--bullet',
                        help='un-numbered items, i.e. use bullets',
                        action='store_true')
    parser.add_argument('-l', '--level',
                        help='max title level to include (default is 3)',
                        type=int, default=3)

    args = parser.parse_args()

    reader = NotebookReader(args.file)
    titles = reader.get_titles()

    contents = reader.generate_contents_table(titles,
                                              numbered=not args.bullet,
                                              max_level=args.level)
    print(contents)


class NotebookReader:

    HTML_LINK_REPLACE_MAPPING = str.maketrans({
        '`': '', ' ': '-'
    })

    def __init__(self, fp):
        with open(fp, 'r') as f:
            self.data = json.load(f)

    @classmethod
    def generate_contents_table(cls, titles, numbered=True, max_level=3):
        """
        :param titles: list of tuples (str title, int level)
        :param bool numbered: whether to number items or use bullet points
        :param int max_level: maximum title level to include in contents table (i.e. number of `#`)
        :return: contents table (string) that can be copied in a markdown cell
        """
        bullet = '1.' if numbered else '*'

        def format_title_name(name):
            # Sanitize characters
            name = name.strip()
            name = name.translate(cls.HTML_LINK_REPLACE_MAPPING)
            return name

        def create_link_entry(title):
            link = format_title_name(title)
            return f'[{title}](#{link})'

        def gen_contents_item(title, level):
            indent = '    ' * (level - 1)
            return indent + f'{bullet} ' + create_link_entry(title)

        entries = [
            gen_contents_item(*e) for e in titles
            if e[1] <= max_level
        ]

        return '\n'.join(entries)

    def get_titles(self):
        """
        :return: list of (str title, int level)
        """
        def detect_headers(markdown_source):
            match_pattern = map(
                lambda src: re.search(r'^\#+ (.+)\n?', src),
                markdown_source
            )
            # Discard None's
            match_pattern = filter(lambda e: e is not None, match_pattern)
            # Extract title and title level
            return [
                (pat.group(1), pat.group(0).split(' ')[0].count('#'))
                for pat in match_pattern
            ]

        markdown = self.get_markdown_cells()
        titles = []
        for cell in markdown:
            headers = detect_headers(cell['source'])
            titles.extend(headers)

        return titles

    def get_markdown_cells(self):
        return [
            cell for cell in self.data['cells'] if cell['cell_type'] == 'markdown'
        ]


if __name__ == '__main__':
    main()

