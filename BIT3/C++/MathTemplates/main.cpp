#include "MathTemplates.h"
#include <iostream>
#include <string>

int main(void)
{
     std::cout<<Add<std::string>("first","second")<<std::endl;    
     std::cout<<Add<float>(2.5f,2.6f)<<std::endl;
     std::cout<<Sub<int>(2.5f,2.6f)<<std::endl;
     std::cout<<Mul<float>(2.5f,2.6f)<<std::endl;
     std::cout<<Div<int>(10,5)<<std::endl;
}