# pman
*This project was made as a part of a course Secure Programming.*

A command line password manager.

- [pman](#pman)
    - [Introduction](#introduction)
    - [Usage examples](#usage-examples)
    - [Structure](#structure)
    - [Secure programming solutions](#secure-programming-solutions)
    - [Testing](#testing)
    - [Implemented / Not implemented](#implemented--not-implemented)
    - [Improvements](#improvements)
  - [Building](#building)
    - [Dependencies](#dependencies)
    - [Build steps](#build-steps)

### Introduction

The pman is a command-line password manager that operates a plain file-based
database, which stores encrypted usernames and passwords inside several
different categories. The pman is operated much like any other command-line
application.

See *docs/pman_cli_man.md* for man-style documentation for the CLI.

### Usage examples

TODO
### Structure

See *docs/design/pman_design.pdf* for full architecture and design description.
The general structure of pman is best viewed from the implementation architecture
view.

To describe the structure really briefly, pman consist of components such as
encryption, hashing, input handling, command-line parsing, database handling
that communicate through interfaces defined in C. The purpose of this structure
is to make handling users' password as secure as possible among other things.

### Secure programming solutions

The OWASP TOP 10 does not quite fit this project as this is not a web application,
so I will not address them here. After all, being a C project, most of the
secure programming solutions with pman are more or less memory related.

I will try to address my solutions in terms of [CERT's Secure Coding Principles](https://wiki.sei.cmu.edu/confluence/display/seccode/Top+10+Secure+Coding+Practices):

1. Validate input

In its current state the only inputs to the system are command-line options
and arguments, and usernames and passwords through prompts. The usernames and
prompts naturally do not require any kind of validation, as the user is free
to choose whatever username or password they like.

The command-line options go through extensive validation during the parsing
phase, which is normal for a CLI application. For what comes to arguments, the
only real problematic input is path argument, which is goes through a whitelisted
character check before translation.

2. Heed compiler warnings

This was one of the primary focuses with pman since the beginning. pman is
compiled with full set of warnings `-Wall -Wextra -Wpedantic` along with
interpreting all warnings as errors (`-Werror`).

Additionally, pman integrates `clang-tidy` and `Clang Static Analysis` in
its build process with every feasible analysis option turned on (more details
in *.clang-tidy* file). The warnings from `clang-tidy` are also interpreted as
errors.

3. Architect and design for security policies.

The database handling subsystem's architecture reflects this. The interface
is designed such that the user's need to unlock (authenticate and verify)
the database before they are able to read it.

4. Keep it simple.

Even though the architecture of pman is not at all small, I still believe the
design was kept to a minimum. There were vast benefits of splitting the
components the way I did. To name a few: flexibility, completeness,
single-responsibility and separating security focused components.

5. Default deny.

No matter how hard one tried, a user is not capable of accessing the database
without a password. This is thanks to multi-layer encryption.

6. Sanitize data sent to other systems.

pman seeks to default-disable password prompt echoing and writing user passwords
to the command-line. This is to disable the command-line history as an attack
vector.

### Testing

Due to time constraints, only extensive manual testing was done. Unit tests
and functional tests will be developed in the future.

### Implemented / Not implemented

* Implemented commands:
  * `new` - The user is able create an encrypted database.
  * `get` - The user is able to fetch entries from the database.
  * `add` - The user is able to add entries to the database.
  * `list` - The user can list all entries of the database.
* Not implemented commands:
  * `login` - Login caching is a nice-to-have feature, which needs to
  * implemented securely.
  * `del` - The internal data structures would need reworking to better
  * support deleting.
  * `edit` - Interactive editing is again a nice-to-have feature.

* Implemented options:
  * `--echo|-e` - The user can control the echoing of the prompts.
  * `--db-path|-d path` - The user can specify a database from the command-line.
* Not implemented options:
  * `--category|-c` - Currently the database only supports a single category.
  * `--clip|-x` - Clipboard integration is not trivial, and requires focus
    from a security POV (password cannot remain in the clipboard).
### Improvements

* Implement the missing commands / options
* Add Windows support?
* Refactor code (always feasible)
* Implement unit and functional tests

## Building

### Dependencies

* Libraries
  * OpenSSL - User for encrypting, decrypting and hashing.
  * argp    - Used for command-line parsing.
  * inih    - Used for parsing the configuration file.
  * A few POSIX and GNU extensions (`O_TMPFILE, strdup, and strtok_r etc.`)
* Build tools
  * CMake
  * C11 supporting compiler (preferably clang)
  * clang-tidy - For linting and static analysis
  * iwyu       - Include what you use (linting tool)
  * lwyu       - Link what you use (linting tool)

### Build steps

Building should be rather simple, as all of the dependencies are handled in the
CMake files. Only the build dependencies are required to be installed.

The repository supports building in CLion and Visual Studio Code IDEs, but
building from the command line is probably the easiest way. Personally I
recommend doing an out-of-source build:

    # In root of pman project, first configure CMake
    mkdir build_debug/
    cmake -Bbuild_debug -DCMAKE_BUILD_TYPE=Debug .

    # 'Release' and 'DebugAsan' build are also supported.

    # Then build
    cd build_debug/
    make -j

    # Run after a successful build
    cd src/
    pman --help
