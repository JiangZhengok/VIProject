#include "stdio.h" 
#include<iostream>
#include<fstream>
#include<algorithm>
using namespace std;
//中值滤波函数 
double medFilter(double* DataBuffer)
{ 
  double temp;
  sort(DataBuffer,DataBuffer+19);
  temp=DataBuffer[9];
  return temp; 
}
//主函数 
int main() 
{ 
  ifstream fin("./test.csv");
  double temp_buf[3002]={0};
  for(auto& d:temp_buf)
  fin>>d;
  double ret;
  double a[2984][19];
  int i,j;
  ofstream write;
  write.open("out.csv");
  for (int i = 0; i<2984; i++) 
  { 
  for(int j=0;j<19;j++)
  {
  a[i][j]=temp_buf[i+j];
  }
    ret=medFilter(&a[i][0]);
    cout<<ret<<" "<<endl;
    
    write<<ret<<endl;  
  }

  write.close();
  return 0;
} 

