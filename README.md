# glfwpp
C++11 stateless translation of the glfw 3 API

This does a perfect 1:1 mapping or binding of the glfw 3 C-style API to C++.   This allows a really simple use of the GLFW3 API in idomatic C++.

It follows an object style naming scheme for the functions, along with strict adherence to RAII.  For example, to make a window instead of `glfwCreateWindow`, you just make a `glfw::Window` object
Then, instead of calling `glfwWindowGetWindowPos(wind,&x,&y)` you simply call `auto v=wind.Pos();` on that window object.   

Set/Get uses the `void Foo(int& a)/int Foo() const` convention in the C++ STL,  and all other functions naturally extend from their counterparts.

Callbacks are setup automatically and map to public `std::function` members for use with lambdas or other function bindings.  For Example, `wind.FocusCallback=[](bool f){};` isntead of `glfwSetWindowFocusCallback`

Minimal Example:

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

