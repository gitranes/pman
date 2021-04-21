
### Name

**`pman`** - saves, generates and retrieves passwords from a secure database.

### Synopsis

**`pman `**`[COMMAND][OPTIONS][ARGS]`

### Description

**pman** is a simple password manager that stores usernames and passwords in
encrypted file-based databases. The database may include different categories
to make grouping the database entries easier. The **pman** allows the user to
manage multiple databases, in which entries can be listed, added, modified,
retrieved and generated.

The way multiple databases are handled with **pman** is that the user logs in
to the database that they want to manage. If the login is successful the user
can then manage entries in this database for a certain period of time
(configurable).

To start use the **new** command to generate a database.

### Commands

**`new `**`db_path`

Creates a new database which passwords a stored. The command then prompts
the user for the configuration of the database, such as master password,
login timeout.

**`login `**`db_path`

Logs in to database that is then used with further commands. The login times
out after a set period of time specified in the configuration.

**`list `**`[db_path]`

List the categories and entries in a database. By default uses the database
that the user has logged on to, but optionally allows the user to select another
database. The same command is aliased with `ls`.

**`add `**`[--echo, -e][db_path][category] user`

Adds a new entry to the current database. By default the password prompt does
not echo, which can be modified with the `--echo` or `-e` option. The category
and database path are optional. Aliases for the command are `insert` and
`append`.

**`del `**`[--force, -f][db_path] user`

Removes an entry from a database. Every category in the database is searched.
By default prompts before removing, which can be modified with the `--force`
or `-f` option. Other aliases include `remove`, `delete` and `rm`.

**`edit `**`[db_path] user`

Modifies an entry in the database. Again, every category in the database is
searched.

### Examples
