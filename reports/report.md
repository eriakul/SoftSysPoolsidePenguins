**What is the goal of your project; for example, what do you plan to make, and what should it do?**

The goal of my project is to learn about networking and apply it to a project. I plan to make a terminal app that allows two computers to connect with a game of rock paper scissors.

**Reflect on the outcomes of your learning goals.**

My learning goal is to learn how to create a full project with the C language and gain basic knowledge about networks. I believe I gained a moderate amount of knowledge about sockets. 

**What resources did you find that were useful to you.**

* [TCP server client implementation (geeksforgeeks)](https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/)
* [How to send it over socket in C (stackoverflow)](https://stackoverflow.com/questions/9140409/transfer-integer-over-a-socket-in-c)

**What were you able to get done?**

The app is complete! When run, two people on the same network can play rock paper scissors. 

**Explain at least one design decision you made.  Were there several ways to do something?  Which did you choose and why?**

I decided to keep messages between the server and client primarily integers so I would not have to deal with decoding string messages.

I also decided to create a global enum object that converted its fields to integers to keep track of all the flags I was sending back and forth between client and server.
