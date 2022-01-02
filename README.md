# IPC queue

This is a study on how to do create a queue via IPC (inter-process communication).

Please take a look at the examples of [producer](./producer.c) and [consumer](./consumer.c)

## Design

There are two main components that can be used independently:

**SharedMem** 

exposes an interface to share memory among processes, 
it uses [MMAP](https://en.wikipedia.org/wiki/Mmap) under the hood, and pre-allocates space that will be used.

---

**SharedQueue** 

It uses the shared memory to create a ring buffer in the available space.
Each message has a sequence number, that is prepended to the front on the message like a header, so we can keep track when reading.

The messages for this implementation have a fixed size, this way we can have O(1) access to memory by calculating the memory offset. 
This has some downsides if your domain has variable-length messages, but this could be easily implemented by modifying the queue and adding the message size into the header.

Due to each process being responsible for managing its own sequence number we end up having a lock-free, single-producer, multi-consumer model.
This is heavily inspired by [LMAX's Disruptor](https://lmax-exchange.github.io/disruptor/).

Be aware that due to the ring buffer reuse of space some  messages can be lost if the consumer is lagging (not ingesting messages fast enough).
This has to be measured for your use case and can be solved by increasing the buffer size of the shared memory. 

---

Warning: those components can act as writer/reader (or producer/consumer) but never both simultaneously.

## How to run

Just run `make` and then start the `./producer` and `./consumer` in different terminals.

## Improvements

I'm not a C expert, and I'm sure this code can be improved a lot

This is just for demonstration purposes; thus, there are lots of cases that are not covered, some examples: 
* trying to write to a read-only resources and so on
* try to change initiate more than once (writer becomes a reader)

Feel free to contribute.

---

Shout-out to @apuxbt for sharing some snippets that inspired me  