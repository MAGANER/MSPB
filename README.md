# MSPB
console utility to store files in one  encrypted box

# The idea of project
I already wrote ```My Secret Personal Diary``` and the name makes the purpose obvious,<br>
so this project provides abilities to store any files in encrypted "boxes" to keep them<br>
from people, who must not have access to the files in the box.<br>

For encryption i use ```AES``` algorithm as i already did in MSPD.

To use utility you should run it from console and then use commands, descripted below.

# Commands
```q``` - exit utility and if you have opened box, it saves it.<br>
```cls``` - clear the screen.<br>

```new``` - takes 1 argument name of new box.<br>
Example:```>>>>new my_box```<br>

```open``` - open box and it have two variants of arguments list.<br>
Example 1:```>>>>open box key_file```<br>
Example 2:```>>>>open box key iv```<br>

```md``` - makes new directory in box and it takes optional arguments list.<br>
Example 1:```>>>>md new_dir```<br>
Example 2:```>>>>md new_dir file1 ... filen```<br>

```add``` - adds files to directory.<br>
Example:```>>>>add dir file1 ... filen```<br>

```sdc``` - shows directories' contents.<br>
Example:```>>>>sdc dir1 ... dirn```<br>

```ls``` - show list of existing directories.<br>
