#include "Cursor.hpp"
using namespace glfw;


Cursor::Cursor(GLFWimage* img,const Point<int>& hot)
{
	glfw::impl::Verify();
	ptr.reset(glfwCreateCursor(img,hot.x,hot.y),glfwDestroyCursor);
	if(!ptr.get()) 
		throw Exception(GLFWPP_ERROR,"Failed to create custom cursor");
}
Cursor::Cursor(int s)
{
	glfw::impl::Verify();
	ptr.reset(glfwCreateStandardCursor(s),glfwDestroyCursor);
	if(!ptr.get()) 
		throw Exception(GLFWPP_ERROR,"Failed to create standard cursor");
}

