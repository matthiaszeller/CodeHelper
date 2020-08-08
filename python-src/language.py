

class Language:
    def __init__(self):
        pass

    @classmethod
    def inline_comment_char(cls) -> str:
        pass

    @classmethod
    def name(cls):
        pass


class Python(Language):
    @classmethod
    def inline_comment_char(cls):
        return '#'

    @classmethod
    def name(cls):
        return 'Python'


class CPP(Language):
    @classmethod
    def inline_comment_char(cls):
        return '//'

    @classmethod
    def name(cls):
        return 'C++'

LANGUAGES = [CPP, Python]
