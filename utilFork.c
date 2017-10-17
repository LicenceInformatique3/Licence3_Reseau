/**
*\file utilFork.c
*\brief 
*\author Gaëtan,Lucas
*\version 0.1
*\date 17 octobre 2017
*/
#include <stdio.h>
#include <sys/wait.h>

void kill_zombies(int sig){
	printf("signal %d capte\n",sig);
	int j;
	while((j=waitpid(-1,NULL,WNOHANG)) > 0)
		printf("Elimination du zombie %d \n",j);
}
