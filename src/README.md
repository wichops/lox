# lox language interpreter 
This is my take on [Crafting Interpreters](https://craftinginterpreters.com) book in C
An effort to keep my programming and computers understading skills sharp

I've only used an LLM for consulting how to setup `meson` and `make`

## Syntax 
```
expression     → literal
               | unary
               | binary
               | grouping ;

literal        → NUMBER | STRING | "true" | "false" | "nil" ;
grouping       → "(" expression ")" ;
unary          → ( "-" | "!" ) expression ;
binary         → expression operator expression ;
operator       → "==" | "!=" | "<" | "<=" | ">" | ">="
               | "+"  | "-"  | "*" | "/" ;

```
