from language import Python, LANGUAGES


class Comment:

    filling_chars = ['#', '-', '=', '+', '^', '>', '<', '/', '|']

    default_options = {
        'filling': '=',
        'filling2': '-',
        'right-char': True,
        'capitalize': True,
        'length': 50
    }

    @classmethod
    def parse_options(cls, data):
        """
        :param str data:
        :return: dict options
        """
        def detect_language(line):
            for l in LANGUAGES:
                c = l.inline_comment_char()
                if line[:len(c)] == c:
                    return l

        options = {}
        lang = None
        at = -1
        data = data.split('\n')
        for i, line in enumerate(data):
            line = line.strip()
            l = detect_language(line)
            if l is not None:
                lang = l
                at = i
                break

        if lang is None or len(data) <= at+2:
            raise ValueError

        l1 = data[at].strip()
        l2 = data[at+1].strip()
        l3 = data[at+2].strip()

        # Length
        N = len(l1)
        if N != len(l2) or N != len(l3) or len(l2) != len(l3):
            # Give another chance
            return cls.parse_options('\n'.join(data[at+1:]))
        options['length'] = N

        # Right-hand side character
        c = lang.inline_comment_char()
        options['right-char'] = (l1[-len(c):] == c)

        # Filling characters
        options['filling'] = l1[int(N/2)]
        options['filling2'] = l2[2]

        # Capitalization
        text = l2.split(' ')[2]
        options['capitalize'] = (text == text.upper())

        return options

    @classmethod
    def gen_comment(cls, text, language, options):
        """
        :param str text: text to format
        :param Language language: programming language
        :param dict options:
        :return: formatted string of a comment
        """
        c = language.inline_comment_char()
        side = ' '
        c2 = c if options['right-char'] else ''
        if options['capitalize']:
            text = text.upper()

        n_filling = options['length'] - 2 * len(side) - 2 * len(c)
        line1 = c + side + (options['filling'] * n_filling) + side + c2
        t = line1 + '\n'

        n = len(text)
        n_side = options['length'] - 2*len(side) - 2*len(c) - 2 - n
        # Force the same number of characters on left and right side
        if n_side % 2 != 0:
            options['length'] += 1
            return cls.gen_comment(text, language, options)

        n_side = int(n_side/2)
        t += c + side + options['filling2'] * n_side + ' ' + text + ' ' + \
             options['filling2'] * n_side + side + c2
        t += '\n' + line1

        return t
