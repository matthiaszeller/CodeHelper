"""Generate a contents table of a jupyter notebook or a Markdown file.
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

    # Sanity check
    if args.file.endswith('.ipynb'):
        reader = NotebookReader(args.file)
    elif args.file.endswith('.md'):
        reader = MarkdownReader(args.file)
    else:
        raise ValueError('Unrecognized file')

    titles = reader.get_titles()

    contents = reader.generate_contents_table(titles,
                                              numbered=not args.bullet,
                                              max_levels=args.level)
    print(contents)


class Reader:

    def __init__(self, fp):
        with open(fp, 'r') as f:
            self.data = self._load_data(f)

    @staticmethod
    def _load_data(file):
        pass

    def get_titles(self):
        pass

    @classmethod
    def generate_contents_table(cls, titles, numbered=True, max_levels=3):
        """
        :param titles: list of tuples (str title, int level)
        :param bool numbered: whether to number items or use bullet points
        :param int max_levels: maximum title level to include in contents table (i.e. number of `#`)
        :return: contents table (string) that can be copied in a markdown cell
        """
        pass


class MarkdownReader(Reader):

    HTML_LINK_REPLACE_MAPPING = str.maketrans({
        '`': '', ' ': '-'
    })

    def get_titles(self):
        """
        :return: list of (str title, int level) in the order found in file
        """
        source = self.data.split('\n')
        return self._detect_headers(source)

    @classmethod
    def generate_contents_table(cls, titles, numbered=True, max_levels=3):
        """
        :param titles: list of tuples (str title, int level)
        :param bool numbered: whether to number items or use bullet points
        :param int max_levels: maximum title level to include in contents table (i.e. number of `#`)
        :return: contents table (string) that can be copied in a markdown cell
        """
        bullet = '1.' if numbered else '*'

        entries = [
            cls._gen_contents_item(*e, bullet) for e in titles
            if e[1] <= max_levels
        ]

        return '\n'.join(entries)

    @classmethod
    def _gen_contents_item(cls, title, level, bullet):
        def format_title_name(name):
            """Sanitize characters"""
            return name.translate(cls.HTML_LINK_REPLACE_MAPPING)

        def create_link_entry(title):
            title = title.strip()
            link = format_title_name(title)
            return f'[{title}](#{link})'

        indent = '    ' * (level - 1)
        return f'{indent} {bullet} {create_link_entry(title)}'

    @staticmethod
    def _detect_headers(markdown_source):
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

    @staticmethod
    def _load_data(file):
        return file.read()


class NotebookReader(MarkdownReader):

    @staticmethod
    def _load_data(file):
        return json.load(file)

    def get_titles(self):
        """
        :return: list of (str title, int level) in the order found in file
        """
        markdown = self.get_markdown_cells()
        titles = []
        for cell in markdown:
            headers = self._detect_headers(cell['source'])
            titles.extend(headers)

        return titles

    def get_markdown_cells(self):
        return [
            cell for cell in self.data['cells'] if cell['cell_type'] == 'markdown'
        ]


if __name__ == '__main__':
    main()

