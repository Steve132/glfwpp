#include "Window.hpp"
#include <bitset>
using namespace glfw;



Window::Window(int width,int height,const Window::Hints& hints,const std::string& title,const Monitor& mon,const Window* share):ptr(nullptr,glfwDestroyWindow)
{
	glfwDefaultWindowHints();
	for(auto& kv : hints)
	{
		switch(kv.second.index())
		{
			case 0:
				glfwWindowHint(kv.first,std::get<int>(kv.second));
				break;
				#if GLFWPP_GLFW_VERSION_FULL >= 303
			case 1:
				glfwWindowHintString(kv.first,std::get<std::string>(kv.second).c_str());
				break;
				#endif
			default:
				break;
		};
	}
	GLFWwindow* wnd=glfwCreateWindow(width,height,title.c_str(),mon.ptr,share->ptr.get());
	ptr=std::unique_ptr<GLFWwindow,void (*)(GLFWwindow*)>(wnd,glfwDestroyWindow);
	glfwSetWindowUserPointer(wnd,this);
}
static inline uint32_t popcount(uint32_t x)
{
	return std::bitset<32>(x).count();
}
static inline uint32_t ctz(uint32_t x)
{
	return popcount((x & -x) - 1);
}
static inline uint32_t gcd(uint32_t u,uint32_t v)
{
	if(u==0) return v;
	if(v==0) return u;
	unsigned i=ctz(u),j=ctz(v);
	u >>= i;v>>=j;
	unsigned k=std::min(i,j);
	while( v != 0 )
	{
		if(u > v) std::swap(u,v);
		u -= v;
		v >>= ctz(v);
	}
	return u << k;
}

void Window::AspectRatio(int n,int d) {
	
	uint32_t cd=gcd(n,d);
	n/=cd;
	d/=cd;
	glfwSetWindowAspectRatio(ptr.get(),n,d);
}
