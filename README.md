# leetcode-lang-select

Select random languages for solving leetcode problem among friends.

![figlet2](https://github.com/user-attachments/assets/754d57a3-e9de-4c0c-bed6-06b83d12f4a9)

## Dependencies

For fancy output, tool uses:

- figlet
- echo

command for its working. Can use `sudo apt install figlet` for installing figlet on
Ubuntu.

## Build

Compile the program with any C++17 compiler:

```bash
g++ -std=c++17 leetcode_lang_select.cpp -o leetcode_lang_select
```

Or just use (on unix systems):

```bash
./build
```

that just maps to above command

## Run

Program reads its input from stdin.

- First line contains how many leetcode participants are there (say n).
- Next n lines denote the names of those participants.

If we have sample input file named `input`:

```
3
Person 1
Person 2
Person 3
```

then run:

```bash
./leetcode_lang_select < ./input
```

or just run after editing `input` file:

```
./run
```

that runs the same command as above.

## Conclusion

Have Fun
