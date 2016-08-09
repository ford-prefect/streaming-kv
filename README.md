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
