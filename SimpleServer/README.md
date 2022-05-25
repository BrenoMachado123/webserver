# SimpleServer
This very SimpleServer will help us understand how to get started and how browser clients make requests.

Working on port 4242, just becase... LOL
***

### How to test this?
Well that's a good question...
Every browser makes the requests differently and they all send different messages.
I encourage you to test this program using Mozila Firefox, it's the browser that makes the simplest request and it will accept the message that we send.
Although you can test this with really any browser, but don't be surprised if the behaviour is weird.
For example, google chrome does something quite weird, it connects, it sends a message, recieves our message and AGAIN it connects but this time doesn't send anything... preety weird, right?

### Getting Started
Start the program

```bash
    make test
```

If for some reason the program fails and it says it can't bind the port 4242 it means that the port 4242 is not available.
In this case you need to kill the process using that port and wait a little...

Use this command to see the process id & kill it
```bash
    lsof -ni | grep 4242
    kill PROCESS_NUMBER
```
After the progam starts you will see something like this:


Now, you can go to any browser and visit [http://localhost:4242/](http://localhost:4242/)
Also, if you prefer, you can use the *telnet* command to emulate a client on the terminal
(you can install telnet on Linux with your package manager and in iOS with HomeBrew ak Brew)
```bash
    telnet localhost 4242
```