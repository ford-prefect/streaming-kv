## Design

- Producer/Server
  - Adds to a key-value store
  - Publishes key additions/updates to all subscribers

- Consumer/Client
  - Subscribes to a server
  - Keeps values corresponding to a key atomically updated
    - Can dispatch a callback when this happens
  - Client can query key, gets a reference to the value
    - Stays valid for as long as it holds the reference

## Protocol

- Client -> Server
  - Subscribe: Tell server we want updates
  - Leave: Tell server we no longer want updates
  - Future: subscribe by key

- Server -> Client
  - Update: Provides key and new value, applies to new and updated values

## Threads

- Server
  - 1 thread to accept connections
  - TBD: use a thread pool and spawn workers for sending updates

- Client
  - 1 thread to listen for updates

## Improvements

- Make core code in to a dynamic library
- Make cerr output optional (don't want libraries spewing logs)
- Move to C++11/C++14 conventions where missing

## Testing

### Manual

Run `test-server` and then connect `nc` as a client:

```
$> nc localhost 4321 > out
```

The server lets you set/update key-value pairs. After a few updates, you can
see the messages from `nc` in the `out` file, using a program like `od` or
`hexedit`.

For example, with this on the server:

```
$> ./test-server
Key to update: a
Value: 1
Key to update: b
Value: 2
Key to update: a
Value: 3
Key to update: ^C
```

you will see:

```
$> od -x out
0000000 0101 0000 0100 0000 6100 0131 0001 0000
0000020 0001 0000 3262 0101 0000 0100 0000 6100
0000040 0033
0000041
```

Byte 1 is the version, the next 32 bits are the key length, the next 32 the
value length, and the acutaly key and value bytes then follow.

### Automated

Automated tests should ideally exist by writing a dummy server + client to make
sure updates propagate as expected.

Stress tests to flood updates would also be good to make sure that we maintain
atomicity guarantees.
