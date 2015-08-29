**Program**: Extending the original Sleeping barber problem

**Author**: Anirudh Ravi

**About**:

In this problem, the original Sleeping-barber problem is extended to a multiple sleeping barber problem where many customers visit a barbershop and receive haircut service in the shop. This uses semaphores (Semaphore APIs) available in Linux platform. 

A barbershop consists of a barber room with 3 barber chairs and a waiting room having separate queues (three such queues) of 10 chairs each for each barber. The barbers are trained for different types of haircuts. And hence, each waiting queue relates to different barbers who are experts in a specific type of haircut. Barbers serve the customers according to the haircut they want.

1. 1st Barber takes 5 sec, 2nd barber takes 10 sec and 3rd barber takes 5 sec each for completing the haircut
2. If there are no customers to be served, all the barbers go to sleep
3. If a customer enters the barbershop and the chairs are occupied, then the customer leaves the shop
4. If the barber is busy but chairs are available, then the customer sits in one of the free chairs in the waiting area
5. If the barbers are asleep, the customer wakes up the barbers
6. A customer after waiting in the original queue for 20 seconds changes his mind and decides to go for another type of hair cut that has least number of people waiting. Possibly, he has other things to do and does not mind having any type of haircut
7. Barbers are paid as per the number of customers they serve

**Executing**:

cc Extended-Sleeping-Barber.c -lpthread -lrt -o ESB

./ESB

