# wizsh [![GitHub license](https://img.shields.io/github/license/Naereen/StrapDown.js.svg)](https://github.com/Thenlie/wizsh/blob/main/LICENSE) [![Buy me a Coffee](https://badgen.net/badge/icon/buymeacoffee?icon=buymeacoffee&label&color=purple)](https://www.buymeacoffee.com/leithen)


[![Open Source](https://badges.frapsoft.com/os/v1/open-source.svg?v=103)](https://opensource.org/)
[![Builds badge](https://github.com/thenlie/wizsh/actions/workflows/makefile.yml/badge.svg)](https://github.com/Thenlie/wizsh/actions/)
[![Forks](https://img.shields.io/github/forks/Thenlie/wizsh)]()
[![Open Issues](https://img.shields.io/github/issues/Thenlie/wizsh)](https://github.com/Thenlie/wizsh/issues?q=is%3Aopen+is%3Aissue)
[![Closed Issues](https://img.shields.io/github/issues-closed/thenlie/wizsh?color=yellow)](https://github.com/Thenlie/wizsh/issues?q=is%3Aissue+is%3Aclosed)

A simple shell written in C. This shell was created as a final project for the Harvard CS50 course. As my first real dive into independent C programming, it was an amazing tool to learn the ins and outs of C, git and how shells function on a lower level. Bash and zsh were the main inspirations for the shell, the latter inspiring the name as well. 

The initial goal was to create a simple command line interface that accepted specific commands and responded to them. As I progressed, I eventually wanted this to become a fully featured shell with git integration, directory navigation and more! 

In the v1.0.0 release, wizsh is capable of most common directory navigation commands as well as file and directory modification. It also contains a number of git commands, enough to maintain a small repository. Be sure to check out the wiki for a full list of commands.

[![Git](https://badgen.net/badge/icon/git?icon=git&label&color=orange)](https://git-scm.com/)
[![Terminal](https://badgen.net/badge/icon/terminal?icon=terminal&label)]()

## Demo

https://drive.google.com/file/d/1PfSqTuwhpWkkiA0RY9vr3tEw6Vch2MEF/view?usp=sharing

# Table of Contents

1. [Installation](#installation)
2. [Contributing](#contributing)
3. [Libraries](#libraries)
4. [Questions](#questions)

## Installation

> PREFACE: Wizsh was built on an M1 Mac with no other systems in mind! This means that there are likely issues when attempting to build and run this on any x86 machine. This would be a great issue to contribute to!  

These instructions assume that you have installed homebrew and all other dependencies. 

### Release

To use wizsh, first install the latest release(link).
Next, simply run the binary `wizsh` located in the `/bin` directory. 

### Local

To run wizsh locally, you will first need to [clone](https://docs.github.com/en/repositories/creating-and-managing-repositories/cloning-a-repository) the repository. 
Next, navigate to the wizsh directory. Typically just `cd wizsh` from wherever the `clone` command was run.
Use the command `make` to build to binary. 
Run the binary by using the command `bin/wizsh`. This is the same as `cd bin && wizsh`.

## Contributing

Wizsh is very open to contributions! If you have a feature request, or bug report, please open an issue with the applicable tag. If you would like to simply create a feature, [fork](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/working-with-forks/about-forks) the repository and add in your changes. Then submit a [pull request](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/about-pull-requests) which will be reviewed and eventually mergered if it meets all contribution requirements. 

I would be absolutely thrilled to contiue working on this with other developers. If you have noticed a long period of inactivity, feel free to reach out to me at the link below to get the current status of this project. 

## Libraries

A handful of libraries were used to create this project. Without them, a lot of features would not have been possible. A list of all included libraries can be found below with links to their websites/documentation. 

- [lcurl](https://curl.se/libcurl/)
- [libgit2](https://libgit2.org/)

## Questions?

If you have any questions about the project you can reach out to me via email or GitHub with the information below. 

>Email: leithen113@gmail.com 

>GitHub: [Thenlie](https://github.com/Thenlie)
