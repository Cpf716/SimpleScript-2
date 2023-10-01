# SimpleScript

## Functions

### accept(fildes: int): array | null

Precondition:   `fildes >= 0`<br>
Postcondition:  returns an array of file descriptors identifying clients of the server `fildes`, otherwise returns `null` if the server has no clients or an error occurs

### array(num: int): array | null

Precondition:   `num >= 1`<br>
Postcondition:  returns an array of null values with count `num`

### client(hostName: string, port: int): int

Precondition:   `count hostName > 0 && port >= 0`<br>
Postcondition:  returns a file descriptor identifying the server at `hostName` and `port`, otherwise returns -1 if an error occurs (blocking)

### close(con | fildes: int): int

Precondition:   `con >= 0 || fildes >= 0`<br>
Postcondition:  returns 0 upon successful closure of the MySQL connection `con` or the socket `fildes`, otherwise returns -1 if an error occurs 

### connect(hostName: string, userName: string, password: string): int

Precondition:   `count hostName > 0 && count userName > 0 && count password > 0`<br>
Postcondition:  returns an integer identifying the MySQL connection to `hostName` given `userName` and `password`, otherwise returns -1 if an error occurs

### exists(path: string): int

Precondition:   `count path > 0`<br>
Postcondition:  returns 1 if a file exists at `path`, otherwise returns 0

### gmt(): string

Precondition:   none<br>
Postcondition:  returns the current GMT time string

### input(): number | string

Precondition:   none<br>
Postcondition:  returns a number or string from stdin

### listen(fildes: int, port: int): int

Precondition:   `fildes >= 0 && port >= 0`<br>
Postcondition:  listens to messages received by the socket `fildes` and forwards them to `port`; returns the file descriptor identifying the listener, otherwise returns -1 if an error occurs

### local(): string

Precondition:   none<br>
Postcondition:  returns the current local time string

### prepareQuery(con: int, sql: string, args...): table

Precondition:   `con >= 0 && count sql > 0`<br>
Postcondition:  returns the result set of a prepared statement query

### prepareUpdate(con: int, sql: string, args...): int

Precondition:   `con >= 0 && count sql > 0`<br>
Postcondition:  returns the number of rows updated by a prepared statement update

### query(con: int, sql: string): table

Precondition:   `con >= 0 && count sql > 0`<br>
Postcondition:  returns the result set of `sql` query

### rand(): int

Precondition:   none<br>
Postcondition:  returns a pseudorandom, unsigned integer

### read(path: string): string
### read(path: string, type: string): string | table

Precondition:   `count path > 0 && array("csv", "tsv", "txt") contains type`<br>
Postcondition:  returns data read from the file at `path`, otherwise returns `undefined` if the file does not exist

### recv(fildes: int): string

Precondition:   `fildes >= 0`<br>
Postcondition:  returns data received from the socket `fildes`, otherwise returns `undefined` if disconnected or an error occurs (blocking)

### remove(path: string): int

Precondition:   `count path > 0`<br>
Postcondition:  returns 1 if the file at `path` is removed, otherwise returns 0

### send(fildes: int, data: string): int

Precondition:   `fildes >= 0 && count data > 0`<br>
Postcondition:  returns the number of bytes sent to the socket `fildes`, otherwise returns 0 if disconnected or -1 if an error occurs

### server(port: int, backlog: int)

Precondition:   `port >= 0 && backlog >= 0`<br>
Postcondition:  starts a server at `port` with the capacity of `backlog`; returns the file descriptor identifying the server, otherwise returns -1 if an error occurs

### setSchema(con: int, schema: string)

Precondition:   `con >= 0 && count schema > 0`<br>
Postcondition:  sets the schema for connection `con`; returns `undefined`

### update(con: int, sql: string)

Precondition:   `con >= 0 && count sql > 0`<br>
Postcondition:  returns the number of rows updated by `sql` query

### write(path: string, data: any): string
### write(path: string, data: any, type: string): string

Precondition:   `count path >= 0 && ((typeOf data === "string" && data !== null) || typeOf data !== "string" && array("csv", "tsv") contains type)`<br>
Postcondition:  writes `data` to file; returns `undefined`
