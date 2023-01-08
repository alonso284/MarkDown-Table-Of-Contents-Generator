# Table of Contents
1. [MarkDown-Table-Of-Contents-Generator](#markdown-table-of-contents-generator) at line 6
2. [Description](#description) at line 10
    1. [TODO](#todo) at line 36

# MarkDown-Table-Of-Contents-Generator

This application is made to automate the creation of table of contents for markdown files, it adds other functinoalities for github markdowns. It only works for mac.

# Installation

Download the executable `mdtc` and store it wherever you want. I recommend storing it at `/usr/bin/`

Add the following to `~/.zshrc`
```zsh
export PATH="absolute_path_to_the_file:$PATH"
```

For example, if you store it at `/usr/bin/`, add
```zsh
export PATH="/usr/bin:$PATH"
```

Then restart the terminal or run:
```zsh
source ~/.zshrc
```

# Description

`-t tab_size`
Specifies the tab size for the title indexing, the default is 4

`-i`
Includes the markdown file inside the table of contents generated

`-line`
Includes the line at which the title is located

`-l`
Adds linking to the file, this only works for github markdowns

`-p prefix`
In case `-l` is activated, it adds a `prefix` to the link. Default is `#`

`-v`
Make creation verbose

`-o`
Overwrite main fail instead of creating new one

`-f`
Force creation of table even if it already exists

## TODO
- Allow user to add a brief description to a header in the table of contents
- Find a way to overwrite table of content
