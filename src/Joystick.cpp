#include "Joystick.hpp"
using namespace glfw;

std::vector<Joystick> Joystick::Joysticks()
{
	glfw::impl::Verify();
	std::vector<Joystick> out;
	for(size_t i=0;i<GLFW_JOYSTICK_LAST+1;i++)
	{
		out.push_back(i);
	}
	return out;
}
