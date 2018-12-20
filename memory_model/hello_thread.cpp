#include<iostream>
#include<thread>

void hello_thread(std::string msg)
{
    std::cout<<"Hello Thread msg: "<<msg<<'\n';
}

int main()
{
  //launch a thread
  std::thread t1(hello_thread, "1 2 3");

  //Join the thread
  t1.join();

return 0;
}
