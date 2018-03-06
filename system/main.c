/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

process	main(void)
{
	kprintf("\nHello World!\n");
	kprintf("\nI'm the first XINU app and running function main() in system/main.c.\n");
	kprintf("\nI was created by nulluser() in system/initialize.c using create().\n");
	kprintf("\nMy creator will turn itself into the do-nothing null process.\n");
	kprintf("\nI will create a second XINU app that runs shell() in shell/shell.c as an example.\n");
	kprintf("\nYou can do something else, or do nothing; it's completely up to you.\n");
	kprintf("\n...creating a shell\n");
	kprintf("\n(Maggioli, Vincent)\n");
	kprintf("\nvmaggiol\n");

		
	resume(create((void*)cpubound, 2048, 1, "cpu1", 0, NULL));
	resume(create((void*)cpubound, 2048, 5, "cpu2", 0, NULL));
	resume(create((void*)cpubound, 2048, 10, "cpu3", 0, NULL));
	resume(create((void*)cpubound, 2048, 15, "cpu4", 0, NULL));
	resume(create((void*)cpubound, 2048, 20, "cpu5", 0, NULL));

	kill(2);	
	
	/*recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;*/
}
