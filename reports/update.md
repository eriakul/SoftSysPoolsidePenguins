**What is the goal of your project; for example, what do you plan to make, and what should it do?  
Identify a lower bound you are confident you can achieve and a stretch goal that is more ambitious.**

The goal of my project is to learn about networking and apply it to a project. I plan to make a terminal app that allows two computers to connect with a game of rock paper scissors.

**What are your learning goals; that is, what do you intend to achieve by working on this project?**

my learning goal is to learn how to create a full project with the C language and gain basic knowledge about networks 

**What do you need to get started?  Have you found the resources you need, do you have a plan to find them, or do you need help?**

First, I will need to learn about networking. I found a comprehensive lesson on network basics that I will watch. I will need to figure out how to connect two computers. 

**What are your first steps?  Describe at least three concrete tasks that you can do immediately, and identify which member of the team will do them.  For each one, what is the "definition of done"; that is, what will you produce to demonstrate that the task is done?**

  1) Receive message from one computer to another
  2) Create text input method for commandline 
  3) Send/receive message 

**-- Update --**

**3) What have you done to get started?  Have you found the resources you need, do you have a plan to find them, or do you need help?  If you found any resources you think I should add to the list on the class web page, please email them to me.**

I have watched [a course on Youtube about networking sockets](https://www.youtube.com/watch?v=C7CpfL1p6y0&ab_channel=rhymu8354) and read up on [implementing them in C](https://www.geeksforgeeks.org/socket-programming-cc/). I have completed getting initial socket communications working.

**4) What are you working on now?  Describe at least three concrete tasks that you are working on, and identify which members of the team are working on them.  For each one, what is the "definition of done"; that is, what will you produce to demonstrate that the task is done?**

1) Edit server to host a game when two players join
  DOD: server.c prints out something when it identifies connections from two different sockets. Sends them back a message to start game. 
2) Prompt rock paper or scissors
  DOD: client.c prints out a prompt and fgets an input
3) Show live results to both players
  DOD: client.c sends input to server and server sends back game results