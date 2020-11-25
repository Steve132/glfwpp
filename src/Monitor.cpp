#include "Monitor.hpp"
#include "General.hpp"
#include "Include.hpp"
#include <iostream>
using namespace glfw;

std::vector<Monitor> Monitor::Monitors()
{
	glfw::impl::Verify();
	int count;
	GLFWmonitor** mts=glfwGetMonitors(&count);
	std::vector<Monitor> mnt;
	mnt.reserve(count);
	for(int i=0;i<count;i++)
	{
		mnt.push_back(mts[i]);
	}
	return mnt;
}
Monitor Monitor::Primary()
{
	glfw::impl::Verify();
	return glfwGetPrimaryMonitor();
}
Monitor Monitor::None()
{
	glfw::impl::Verify();
	return nullptr;
}
std::unordered_map<GLFWmonitor*,Monitor::connected_callback_type> Monitor::cbmap;
void Monitor::callbackDispatch(GLFWmonitor* m,int connected)
{
	Monitor mon(m);
	if(cbmap.count(m)) cbmap[m](mon,connected == GLFW_TRUE);
}


GLFWvidmode Monitor::VideoMode() const
{
	return *glfwGetVideoMode(ptr);
}
std::vector<GLFWvidmode> Monitor::VideoModes() const
{
	int count;
	const GLFWvidmode* mds=glfwGetVideoModes(ptr,&count);
	return std::vector<GLFWvidmode>(mds,mds+count);
}

void Monitor::Gamma(const Monitor::Ramp& r)
{
	if(r.red.size() == r.green.size() == r.blue.size()){
		GLFWgammaramp gr;
		gr.red=const_cast<uint16_t*>(r.red.data());
		gr.green=const_cast<uint16_t*>(r.green.data());
		gr.blue=const_cast<uint16_t*>(r.blue.data());
		gr.size=(unsigned int)r.red.size();
		glfwSetGammaRamp(ptr,&gr);
	}
	else{
		throw glfw::Exception(GLFWPP_ERROR,"Supplied gamma ramp has inconsistent sizes");
	}
}

Monitor::Ramp Monitor::Gamma() const
{
	const GLFWgammaramp* grp=glfwGetGammaRamp(ptr);
	Monitor::Ramp rmp;
	rmp.red.assign(grp->red,grp->red + grp->size);
	rmp.green.assign(grp->green,grp->green + grp->size);
	rmp.blue.assign(grp->blue,grp->blue + grp->size);
	return rmp;
}

std::ostream& operator<<(std::ostream& out,const GLFWvidmode& gvm)
{
	return out << gvm.width << "x" << gvm.height << "@" << gvm.refreshRate 
		<< ":" << gvm.redBits << "r" << gvm.greenBits << "g" << gvm.blueBits << "b";
}
std::istream& operator>>(std::istream& inp,GLFWvidmode& gvm)
{
	inp >> gvm.width;inp.ignore();
	inp >> gvm.height;inp.ignore();
	inp >> gvm.refreshRate;inp.ignore();
	inp >> gvm.redBits;inp.ignore();
	inp >> gvm.greenBits;inp.ignore();
	inp >> gvm.blueBits;inp.ignore();
	return inp;
}
