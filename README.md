 
![Holberton School Logo](https://apply.holbertonschool.com/holberton-logo.png)

Welcome to the Holberton School Simple Shell project, a custom shell implementation designed and developed by Belinda Shan and Eva Micich.

This project is part of the curriculum at [Holberton School]([https://www.holbertonschool.com/](https://www.holbertonschool.com/)) and is intended to demonstrate the knowledge of Unix system programming, specifically in writing a simple UNIX command interpreter.

## Table of Contents

- [Features](#features)

- [Installation](#installation)

- [Usage](#usage)

- [Examples](#examples)

- [Flow chart](#flowchart)

## Features

- Supports built-in commands: `exit` and `env`, 

- Handles command execution with the `$PATH` environment variable.

- Supports redirection and pipes.

- Implements basic error handling.

## Installation

To install the Holberton School Simple Shell, follow these steps:

1. Clone the repository:

```

git clone https://github.com/EvaMicich/holbertonschool-simple_shell.git

```

2. Change to the project directory:

```

cd holbertonschool-simple_shell

```

3. Compile the source code using the supplied makefile:

```

make

```


## Usage

To start the Holberton School Simple Shell, run:

```

./hsh

```

You will be presented with a shell prompt, where you can enter commands.

## Examples

Here are some example commands that you can try in the Holberton School Simple Shell:

### Interactive Mode

​​Using the ls command with -l option 
  
```

#cisfun$ /bin/ls -l
FINISH OUTPUT HERE

```

  ### Non-Interactive Mode

```

echo "/bin/ls" | ./hsh
FINISH OUTPUT HERE

```

  
  ## Flowchart
  The flow of the program
![image](https://user-images.githubusercontent.com/124496441/234749662-930ae8f1-63d5-42da-99f1-671a443e590f.png)
