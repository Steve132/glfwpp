#include<iostream>
#include<utility>

#include <GLFW.hpp>

int main(void)
{
	glfw::Window::Hints hnts;
	glfw::Window wind(640,480,hnts,"Simple");
	
	wind.MakeContextCurrent();
	while(wind)
	{
		wind.SwapBuffers();
		glfw::Events::Poll();
	}
	return 0;
}

