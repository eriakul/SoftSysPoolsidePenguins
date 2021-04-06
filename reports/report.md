1) What is the goal of your project; for example, what do you plan to make, and what should it do?

2) What are your learning goals; that is, what do you intend to achieve by working on this project?

3) What resources did you find that were useful to you.  If you found any resources you think I should add to the list on the class web page, please email them to me.

4) What were you able to get done?  Include in the report whatever evidence is appropriate to demonstrate the outcome of the project.  Consider including snippets of code with explanation; do not paste in large chunks of unexplained code.  Consider including links to relevant files.  And do include output from the program(s) you wrote.

5) Explain at least one design decision you made.  Were there several ways to do something?  Which did you choose and why?

6) You will probably want to present a few code snippets that present the most important parts of your implementation.  You should not paste in large chunks of code or put them in the Appendix.  You can provide a link to a code file, but the report should stand alone; I should not have to read your code files to understand what you did.

7) Reflect on the outcome of the project in terms of your learning goals.  Between the lower and upper bounds you identified in the proposal, where did your project end up?  Did you achieve your learning goals?

Audience: Target an external audience that wants to know what you did and why.  More specifically, think about students in future versions of SoftSys who might want to work on a related project.  Also think about people who might look at your online portfolio to see what you know, what you can do, and how well you can communicate.

**What is the goal of your project; for example, what do you plan to make, and what should it do?  
Identify a lower bound you are confident you can achieve and a stretch goal that is more ambitious.**

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
