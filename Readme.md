# OSHW1
## Shared memory using POSIX API
##Screenshot
###Server side
![preview](s.png)
###Client side
![preview](c.png)
##Instruction
### Build
	gcc shm_client.c -o shc && gcc shm_server.c -o shs
### Run
	./shc && ./ shs

###Client side
If you want to add a data "apple", you need to input %apple.

% is to store.

	=> %apple
If you want to print out all data you stored in server, use '$'.

	=> $
	
###Server side
Server is where you show your data and show if the input data exist or not.