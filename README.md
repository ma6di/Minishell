🐚 Minishell - A Simple Unix Shell

🚀 Overview

Minishell is a miniature Unix shell developed as part of the 42 school curriculum.  
It replicates many behaviors of Bash, including environment handling, command execution, redirections, pipes, and built-in commands — all while offering deeper insights into how a shell works under the hood.

🤝 Collaborator
This project was made possible through the collaboration of:
  - @Nat-Ma
---
📦 Features

🧠 Lexing, Tokenization & Parsing

A core part of any shell is transforming raw user input into structured, executable instructions. Minishell uses a multi-phase approach:

    🔹 Lexing
      - Raw input is scanned character-by-character and broken into the smallest meaningful units, 
        such as strings, operators (`|`, `>`, `>>`, etc.), and control characters.

    🔹 Tokenization
      Lexemes are categorized into **tokens** like:
      - `WORD` (command or argument)
      - `PIPE`
      - `REDIRECT_IN`, `REDIRECT_OUT`, `HEREDOC`, etc.
      - `ENV_VAR`
        
    🔹 Parsing
      Tokens are assembled into an **abstract syntax tree (AST)** or command table, which reflects the logical structure of the input. This allows:
      - Grouping of commands with pipes
      - Nesting for redirections
      - Handling heredocs
      - Command sequences
    
    🔹 Prompt & Input
      - Custom shell prompt displaying user input.
      - Signal handling (`Ctrl+C`, `Ctrl+D`, `Ctrl+\`) as in Bash.
      - Command history via arrow keys (readline).
      
    🔹 Environment variable expansion

🧠 Execution

    🔹 Built-ins
      - `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`.

    🔹 Redirections
      - `>`, `>>`, `<`, and `<<` (heredoc).

    🔹 Pipes
      - Support for `|` to connect commands.

    🔹 Environment
     - Inheritance of environment variables.
     - Proper management via `export` / `unset`.

  🔹 Execution
    - PATH resolution.
    - Executing both absolute and relative paths.

