
#ifndef GLFWPP_CURSOR_HPP
#define GLFWPP_CURSOR_HPP
//Joystick Callback
#include "Include.hpp"
#include "General.hpp"
#include <memory>

namespace glfw
{
	class Window;
	class Cursor
	{
	protected:
		std::shared_ptr<GLFWcursor> ptr;
		friend class Window;
	public:
		Cursor(int standard);
		Cursor(GLFWimage* img,const Point<int>& hot=Point<int>{0,0});
	};
}

#endif
