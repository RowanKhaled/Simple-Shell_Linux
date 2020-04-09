


#include <iostream>
#include <cstring>
#include <unistd.h>
#include<stdio.h>
#include<sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fstream>
#include <signal.h>

using namespace std;

int flag;

void HandlerFunction(int number)

{
   ofstream outFile ("TheFile.log",ios::app);

   outFile<<"child process has been terminated"<<endl;

outFile.close();
}


int main()

{
	string output[100];  // will be my array of strings after strtok
    char* tokchar;       // pointer to next element in the string
    char str[100];      //input string of the user (the command)
    int theSize;      // size of the new array after strtok


    while (true)
    {
		 flag = 0;
		 theSize=0;
        cout<< "enter command:"<<endl;

        cin.getline(str,100);      //input of user


        tokchar= strtok(str," "); //split when there is a space
        while(tokchar!=NULL)      //loop till end of the string
        {
            output[theSize]=tokchar; //array of strings after strtok
            tokchar = strtok (NULL," ");
            theSize++;

        }
        if(output[0]== "exit" )
        {
            exit(0);

        }
        if (output[theSize-1]=="&")  //if & is found in the last index of the array , set flag=1
        {
                flag=1;

        }

        if(output[0]=="cd") //if cd is found in the first index of the array , set flag=2
        {flag=2;

        }

        char* args[theSize];  //should cast from string to char*

        for (int j=0; j<theSize; j++) //casting all non empty elements of the array
        {
           args[j]=(char*)output[j].c_str();

        }
          if(flag==1)    //if flag=1 so & is found should replace it with NULL
            args[theSize-1]=NULL;


           for (int i =theSize ;i<100 ; i++ )  //replacing the garbage of the unused array with NULL
			args[theSize]=NULL;


    if (flag==2){    //if flag=2 so cd is found

        chdir(args[1]); //send path to chdir();


        }


    pid_t pID = fork();

      if (flag == 0)   //FIRST CASE: No & (Parent will wait)
      {
        if( pID < 0 )   //unsuccessful
        {

            perror ("error fork");
        }
		else            //successful fork
		{
        	if (pID ==0)   //child
        	{

            		int returnedValue = execvp( args[0], args);
            		if(returnedValue==-1)
            		{

                		perror ("execution failed");
            		}

    			exit(0);

        	}
			else  //parent
			{
                signal(SIGCHLD,HandlerFunction);

				wait(NULL);
			}

		}
      }
     else if(flag==1)      //SECOND CASE flag=1, there is &, NO wait
      {
        	  if( pID < 0 )   //unsuccessful
        {
             perror ("error fork");
        }
		else
		{
        	if (pID ==0)   //child
        	{
                	int returnedValue =execvp( args[0], args);
            		if(returnedValue==-1)
            		{
                		perror ("execution failed");
            		}
    				exit(0);

        	}
			else
			{
                signal(SIGCHLD,HandlerFunction);

			}

		}

      }


    }

    return 0;
}
