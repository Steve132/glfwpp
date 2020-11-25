#include "Window.hpp"
#include <bitset>
#include "Cursor.hpp"
using namespace glfw;



Window::Window(int width,int height,const Window::Hints& hints,const std::string& title,const glfw::Monitor& mon,const Window* share):ptr(nullptr,glfwDestroyWindow)
{
	glfw::impl::Verify();
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
	GLFWwindow* wnd=glfwCreateWindow(width,height,title.c_str(),mon.ptr,share ? share->ptr.get() : nullptr);
	if(!wnd) throw glfw::Exception(GLFWPP_ERROR,"Failed to create window!");
	ptr=std::unique_ptr<GLFWwindow,void (*)(GLFWwindow*)>(wnd,glfwDestroyWindow);
	glfwSetWindowUserPointer(wnd,this);
	
	glfwSetWindowPosCallback(wnd,dispatchPosCallback); 
	PosCallback=[](const Point<int>&){};
	glfwSetWindowSizeCallback(wnd,dispatchSizeCallback);
	SizeCallback=[](const Shape<int>&){};
	glfwSetWindowCloseCallback(wnd,dispatchCloseCallback);
	CloseCallback=[](){};
	glfwSetWindowRefreshCallback(wnd,dispatchRefreshCallback);
	RefreshCallback=[](){};
	glfwSetWindowFocusCallback(wnd,dispatchFocusCallback);
	FocusCallback=[](bool){};
	glfwSetWindowIconifyCallback(wnd,dispatchIconifyCallback);
	IconifyCallback=[](bool){};
	glfwSetFramebufferSizeCallback(wnd,dispatchFramebufferSizeCallback);
	FramebufferSizeCallback=[](const Shape<int>&){};	
	
#if GLFWPP_GLFW_VERSION_FULL >= 303
	glfwSetWindowMaximizeCallback(wnd,dispatchMaximizeCallback);
	MaximizeCallback=[](bool){};
	glfwSetWindowContentScaleCallback(wnd,dispatchContentScaleCallback);
	ContentScaleCallback=[](const Point<float>&){};
#endif
	
	glfwSetMouseButtonCallback(wnd,dispatchMouseButtonCallback);
	MouseButtonCallback=[](int,int,int){};
	glfwSetCursorPosCallback(wnd,dispatchCursorPosCallback);
	CursorPosCallback=[](const Point<double>&){};
	glfwSetCursorEnterCallback(wnd,dispatchCursorEnterCallback);
	CursorEnterCallback=[](bool){};
	glfwSetScrollCallback(wnd,dispatchScrollCallback);
	ScrollCallback=[](const Point<double>&){};
	glfwSetKeyCallback(wnd,dispatchKeyCallback);
	KeyCallback=[](int,int,int,int){};
	glfwSetCharCallback(wnd,dispatchCharCallback);
	CharCallback=[](unsigned int){};
	//static void dispatchCharModsCallback(GLFWwindow*,unsigned int,int); Deprecated
	glfwSetDropCallback(wnd,dispatchDropCallback);
	DropCallback=[](const std::vector<std::string>&){};
	
	std::function<void (const Shape<int>&)> SizeCallback;
	std::function<void ()> CloseCallback;
	std::function<void ()> RefreshCallback;
	std::function<void (bool)> FocusCallback;
	std::function<void (bool)> IconifyCallback;
	std::function<void (bool)> MaximizeCallback;
	std::function<void (const Shape<int>&)> FramebufferSizeCallback;
	std::function<void (const Point<float>&)> ContentScaleCallback;
	
	std::function<void (int,int,int)> MouseButtonCallback;
	std::function<void (const Point<double>&)> CursorPosCallback;
	std::function<void (bool)> CursorEnterCallback;
	std::function<void (const Point<double>&)> ScrollCallback;
	std::function<void (int,int,int,int)> KeyCallback;
	std::function<void (unsigned int)> CharCallback;
	std::function<void (const std::vector<std::string>&)> DropCallback;
}
static inline uint32_t popcount(uint32_t x)
{
	return (uint32_t)std::bitset<32>(x).count();
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

void Window::Cursor(const glfw::Cursor& cur)
{
	if(cur.ptr && ptr) glfwSetCursor(ptr.get(),cur.ptr.get());
}


static inline Window* mkwnd(GLFWwindow* wptr)
{
	return reinterpret_cast<Window*>(glfwGetWindowUserPointer(wptr));
}

void Window::dispatchPosCallback(GLFWwindow* window, int x,int y){
	mkwnd(window)->PosCallback(Point<int>{x,y});
}
void Window::dispatchSizeCallback(GLFWwindow* window, int w,int h){
	mkwnd(window)->SizeCallback(Shape<int>{w,h});
}
void Window::dispatchCloseCallback(GLFWwindow* window){
	mkwnd(window)->CloseCallback();
}
void Window::dispatchRefreshCallback(GLFWwindow* window){
	mkwnd(window)->RefreshCallback();
}
void Window::dispatchFocusCallback(GLFWwindow* window, int focus){
	mkwnd(window)->FocusCallback(focus==GLFW_TRUE);
}
void Window::dispatchIconifyCallback(GLFWwindow* window, int ic){
	mkwnd(window)->IconifyCallback(ic==GLFW_TRUE);
}
void Window::dispatchMaximizeCallback(GLFWwindow* window, int mx){
	mkwnd(window)->MaximizeCallback(mx==GLFW_TRUE);
}
void Window::dispatchFramebufferSizeCallback(GLFWwindow* window, int w,int h){
	mkwnd(window)->FramebufferSizeCallback(Shape<int>{w,h});
}
void Window::dispatchContentScaleCallback(GLFWwindow* window,float x,float y){
	mkwnd(window)->ContentScaleCallback(Point<float>{x,y});
}

void Window::dispatchMouseButtonCallback(GLFWwindow* window,int a,int b,int c){
	mkwnd(window)->MouseButtonCallback(a,b,c);
}
void Window::dispatchCursorPosCallback(GLFWwindow* window,double x,double y){
	mkwnd(window)->CursorPosCallback(Point<double>{x,y});
}
void Window::dispatchCursorEnterCallback(GLFWwindow* window,int e){
	mkwnd(window)->CursorEnterCallback(e==GLFW_TRUE);
}
void Window::dispatchScrollCallback(GLFWwindow* window,double x,double y){
	mkwnd(window)->ScrollCallback(Point<double>{x,y});
}
void Window::dispatchKeyCallback(GLFWwindow* window,int a,int b,int c,int d){
	mkwnd(window)->KeyCallback(a,b,c,d);
}
void Window::dispatchCharCallback(GLFWwindow* window,unsigned int ch){
	mkwnd(window)->CharCallback(ch);
}
//void Window::dispatchCharModsCallback(GLFWwindow*,unsigned int,int){} DEPRECATED
void Window::dispatchDropCallback(GLFWwindow* window,int count,const char* paths[]){
	std::vector<std::string> vpaths(paths,paths+count);
	mkwnd(window)->DropCallback(vpaths);
}
