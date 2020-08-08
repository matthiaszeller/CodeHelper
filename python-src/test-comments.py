from comments import Comment
from language import Python, CPP


options = {
    'filling': '=',
    'filling2': '-',
    'right-char': True,
    'capitalize': True,
    'length': 30
}

lang = Python()
print(Comment.gen_comment('header', lang, options))



data = """dummy text
# ===================================== 
# ================= d ================= 
# ===================================== 
other dummy text"""
print()
print(Comment.parse_options(data))


data = """# ============= #
# --- DUMMY --- #
# ============= #"""
print()
print(Comment.parse_options(data))
