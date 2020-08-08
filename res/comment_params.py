

import sys

### REMINDER : 
# struct CommentParams {
#     QString fillingChar;
#     QString fillingChar2;
#     QString spacingChar;
#     bool commentCharBoth;
#     bool capitalize;
#     CommentStyle style;
#     int length;
# };

# style: number of commented lines

# ARGUMENTS: first is the string to analyze
# the other are derived from the QStringList LANGUAGE_COMMENT_STR

def fail(): 
	print("failure")
	exit()

def output(p) : 
	"""Output the parameters (arg p, dictionnary) in the correct order.
	That is, in the order that is recognized by the class Comment in the C++ code"""

	# For now, we simply use the same order as the values are defined in the struct
	# that is: filling1, filling2, spacing (not implemented), both, capitalize, style, len
	l = [p['filling'], p['filling2'], '', p['both'],
		 p['capitalize'], p['style'], p['length']]
	l = [str(item) for item in l]

	print(",".join(l))

def process(text, com_chars) :
	"""Extract information from text and output the results in the form v1,v2,...,vn.
	In case of failure: output `failure`"""
	if len(com_chars) == 0 : 
		fail()

	# Initialize parameters
	p = {}

	# First, find the programming language 
	# Initialize list (same size as com_chars) to detect which on is used
	idx_lang = [-1] * len(com_chars)
	for i,c in enumerate(com_chars) :
		idx_lang[i] = text.find(c)

	# In case we detected several languages, we take the first one
	# (maybe some chars were detected but were actually in the text of the comment)
	which_lang = -1
	for i, idx in enumerate(idx_lang) :
		if idx != -1 :
			which_lang = i
			break
	if which_lang == -1 : 
		fail()
	# which_lang represents the index of the detected language (in com_chars)
	com_char = com_chars[which_lang]

	######### Number of comment lines
	# First and last index of comment chars encountered
	first_idx = idx_lang[which_lang]
	last_idx = text.rfind(com_char)
	# Substring of interest
	# Manage the fact the maybe the last_idx is the same as first_idx
	if last_idx == -1 or last_idx == first_idx :
		t = text[first_idx:]
	else :
		t = text[first_idx:last_idx+len(com_char)] # must add length of comchar to include it
	splitted = t.split("\n")
	p['style'] = len(splitted)
	if p['style'] == 0 : fail()

	######### Length
	p['length'] = len(splitted[0])

	######### Filling char 1
	# Detect the character of first line, count len(com_char) + a space ...
	p['filling'] = splitted[0][len(com_char) + 1]

	######### Filling char 2 
	# If style of length 1, same as filling1
	# Otherwise, take second line
	# TODO : manage the heavy style ...
	if p['style'] == 1 : 
		p['filling2'] = p['filling']
	else :
		p['filling2'] = splitted[1][len(com_char) + 1]

	######### Comment char on both sides
	# Take first line as reference
	right_idx = splitted[0].rfind(com_char)
	# If same as left index, there's no comment char on both sides
	if right_idx == splitted[0].find(com_char) :
		p['both'] = False 
	else :
		p['both'] = True

	########## Capitalization
	# The pseudo-default value is True
	# First we have to determine the line that contains the text
	# TODO : manage heavy style
	textLine = splitted[0] if p['style'] == 1 else splitted[1]
	# Look for the spaces around the text in textLine
	left_idx = textLine.find(p['filling2'] + " ")
	right_idx = textLine.rfind(" " + p['filling2'])
	if left_idx == -1 or right_idx == -1 :
		fail()
	# Make in sort that the indices surround the text (no filling char and no space)
	# We only correct for the left index, since find returns index from the leftmost char
	left_idx += len(p['filling2']) + 1
	txt = textLine[left_idx:right_idx]
	p['capitalize'] = False if txt.lower() == txt else True # Kind of true by default

	return p

def main(test=-1):
	# LOAD VARIABLES
	# Test implementation : load variables in a different manner
	if(test != -1) :
		text = test[0]
		com_chars = test[1]
	else:
		text = sys.argv[1]
		com_chars = sys.argv[2:]

	# Process the text
	p = process(text, com_chars)

	if(test != -1) : 
		print(text)
		print(p)
	# We now have to output the results in the correct order
	# We use a dedicated function to do so
	output(p)

if __name__ == "__main__" : 

	main()

	exit()

	# Testing

	# Properties :
	# - language: python
	# - length: 15
	# - fillingChar: =
	# - fillingChar2: -
	# - commentCharBoth: true
	# - capitalize: false
	# - style: default
	# TODO: spacingChar
	t = "some text\n# =========== #\n\
# ---- v ---- #\n\
# =========== #\nsome other text"

	t2 = "# === k === "

	main(test=[t, ["#", "//"]])

