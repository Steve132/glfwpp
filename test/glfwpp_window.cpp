#include<iostream>
#include<utility>

#include <GLFW.hpp>

int main(void)
{
	glfw::Window::Hints hnts;
	glfw::Window wind(640,480,hnts,"Simple");
	while(wind)
	{
		wind.SwapBuffers();
		glfw::Events::Poll();
	}
	return 0;
}

