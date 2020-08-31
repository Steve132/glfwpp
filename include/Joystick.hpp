#ifndef GLFWPP_JOYSTICK_HPP
#define GLFWPP_JOYSTICK_HPP
//Joystick Callback
#include "Include.hpp"
#include "General.hpp"

namespace glfw
{
class Joystick
{
protected:
	uint32_t handle;
	Joystick(uint32_t h):handle(h){}
	mutable std::vector<float> axesHolder;
	mutable std::vector<uint8_t> buttonHolder;
	mutable std::vector<uint8_t> hatHolder;
	mutable std::string name;
	mutable std::string guid;
public:
	static std::vector<Joystick> Joysticks();
	bool Present() const {
		return glfwJoystickPresent(handle)==GLFW_TRUE;
	}
	const std::vector<float>& Axes() const {
		int acount;
		const float* aptr=glfwGetJoystickAxes(handle,&acount);
		axesHolder.assign(aptr,aptr+acount);
		return axesHolder;
	}
	const std::vector<uint8_t>& Buttons() const {
		int bcount;
		const uint8_t* bptr=glfwGetJoystickButtons(handle,&bcount);
		buttonHolder.assign(bptr,bptr+bcount);
		return buttonHolder;
	}
	const std::vector<uint8_t>& Hats() const {
		int hcount;
		const uint8_t* hptr=glfwGetJoystickButtons(handle,&hcount);
		hatHolder.assign(hptr,hptr+hcount);
		return hatHolder;
	}
	const std::string& Name() const{
		name.assign(glfwGetJoystickName(handle));
		return name;
	}
#if GLFWPP_GLFW_VERSION_FULL >= 303
	const std::string& GUID() const{
		guid.assign(glfwGetJoystickGUID(handle));
		return guid;
	}
#endif
};
}

#endif
