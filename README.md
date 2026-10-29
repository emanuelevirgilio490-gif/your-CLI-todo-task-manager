# Productivity CLI

Hi! I'd like to introduce you to a project I've always wanted to build.

## What was the problem?

I spend a lot of time in the terminal, so I tried using a TODO task manager. Honestly, **I don't like having to learn someone else's tools**, so I decided to create my own!

## What does it do?

Productivity CLI is a simple and convenient place to plan your day using quick terminal commands! There is also a TUI, however is still in development

## How to install it

You need CMake 3.28 or later to build this project.

You also need a C++ compiler that supports the features used by the project, along with the required dependencies, including `nlohmann/json.hpp`.

To build and run it, execute:

```bash
./exec.sh
```

from the project directory.

## AI usage

I used AI only for the readline and auto completion stuff. Why? Well because
the doc is useless or it's hard to follow and I honestly a little bit Vibe Coded
this stuff. Not because I like Vibe Coding but because readline, in my opinion
is THE WORSE! I'm sorry, but I need to say that the stress and poor doc...
Maybe I'm too much stressed for taking 1h and 25m to implement readline -_-

The reference examples I used are available in the `test/` directory.
