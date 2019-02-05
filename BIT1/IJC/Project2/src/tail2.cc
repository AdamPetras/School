/*
 * tail2.cc
 *
 *  Created on: 7. 4. 2017
 *    Solution: IJC-DU2, příklad
 *      Author: Adam Petráš, FIT
 *    Compiled: G++ 5.4.0 20160609
 */

#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <queue>

using namespace std;
int tail_read(int num_line,int line_count,FILE * pFile,int isprint);
int tail_read(int num_line,int line_count,FILE * pFile,int isprint)
{
	char line[1024];
	int i=0;
	while(fgets(line, 1024, pFile)!= NULL)
	{
		i++;
		if(i>=line_count-num_line+1)
		{
			if(isprint)
				printf("%s", line);
		}
	}
	return i;
}
int main(int argv, char **argc)
{
				if(argv==2)
				{
					FILE *pFile= fopen (argc[argv-1],"r");
					int num = tail_read(0,1,pFile,0);
					fseek(pFile,0,SEEK_SET);
					tail_read(10,num,pFile,1);
					fclose(pFile);

				}
				else
				{
					unsigned int n = 10;
					if(argv > 2 && strcmp(argc[argv-2],"-n")==0)
					{
						n = atoi(argc[argv-1]);
					}
								std::ios::sync_with_stdio(false);
								std::string line;
								std::queue<string> line_stack;
								while(std::getline(std::cin, line))
								{
										 line_stack.push(line);
								}
								for(unsigned int i = line_stack.size();i>0;i--)
								{
									if(n>=i)
									{
										cout << line_stack.front()<<endl;
									}
									line_stack.pop();
								}
				}
  return 0;
}
