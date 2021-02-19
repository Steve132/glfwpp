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
	PosCallback=[](Window&,const Point<int>&){};
	glfwSetWindowSizeCallback(wnd,dispatchSizeCallback);
	SizeCallback=[](Window&,const Shape<int>&){};
	glfwSetWindowCloseCallback(wnd,dispatchCloseCallback);
	CloseCallback=[](Window&){};
	glfwSetWindowRefreshCallback(wnd,dispatchRefreshCallback);
	RefreshCallback=[](Window&){};
	glfwSetWindowFocusCallback(wnd,dispatchFocusCallback);
	FocusCallback=[](Window&,bool){};
	glfwSetWindowIconifyCallback(wnd,dispatchIconifyCallback);
	IconifyCallback=[](Window&,bool){};
	glfwSetFramebufferSizeCallback(wnd,dispatchFramebufferSizeCallback);
	FramebufferSizeCallback=[](Window&,const Shape<int>&){};	
	
#if GLFWPP_GLFW_VERSION_FULL >= 303
	glfwSetWindowMaximizeCallback(wnd,dispatchMaximizeCallback);
	MaximizeCallback=[](Window&,bool){};
	glfwSetWindowContentScaleCallback(wnd,dispatchContentScaleCallback);
	ContentScaleCallback=[](Window&,const Point<float>&){};
#endif
	
	glfwSetMouseButtonCallback(wnd,dispatchMouseButtonCallback);
	MouseButtonCallback=[](Window&,int,int,int){};
	glfwSetCursorPosCallback(wnd,dispatchCursorPosCallback);
	CursorPosCallback=[](Window&,const Point<double>&){};
	glfwSetCursorEnterCallback(wnd,dispatchCursorEnterCallback);
	CursorEnterCallback=[](Window&,bool){};
	glfwSetScrollCallback(wnd,dispatchScrollCallback);
	ScrollCallback=[](Window&,const Point<double>&){};
	glfwSetKeyCallback(wnd,dispatchKeyCallback);
	KeyCallback=[](Window&,int,int,int,int){};
	glfwSetCharCallback(wnd,dispatchCharCallback);
	CharCallback=[](Window&,unsigned int){};
	//static void dispatchCharModsCallback(GLFWwindow*,unsigned int,int); Deprecated
	glfwSetDropCallback(wnd,dispatchDropCallback);
	DropCallback=[](Window&,const std::vector<std::string>&){};
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
	Window* wptr=mkwnd(window); wptr->PosCallback(*wptr,Point<int>{x,y});
}
void Window::dispatchSizeCallback(GLFWwindow* window, int w,int h){
	Window* wptr=mkwnd(window); wptr->SizeCallback(*wptr,Shape<int>{w,h});
}
void Window::dispatchCloseCallback(GLFWwindow* window){
	Window* wptr=mkwnd(window); wptr->CloseCallback(*wptr);
}
void Window::dispatchRefreshCallback(GLFWwindow* window){
	Window* wptr=mkwnd(window); wptr->RefreshCallback(*wptr);
}
void Window::dispatchFocusCallback(GLFWwindow* window, int focus){
	Window* wptr=mkwnd(window); wptr->FocusCallback(*wptr,focus==GLFW_TRUE);
}
void Window::dispatchIconifyCallback(GLFWwindow* window, int ic){
	Window* wptr=mkwnd(window); wptr->IconifyCallback(*wptr,ic==GLFW_TRUE);
}
void Window::dispatchMaximizeCallback(GLFWwindow* window, int mx){
	Window* wptr=mkwnd(window); wptr->MaximizeCallback(*wptr,mx==GLFW_TRUE);
}
void Window::dispatchFramebufferSizeCallback(GLFWwindow* window, int w,int h){
	Window* wptr=mkwnd(window); wptr->FramebufferSizeCallback(*wptr,Shape<int>{w,h});
}
void Window::dispatchContentScaleCallback(GLFWwindow* window,float x,float y){
	Window* wptr=mkwnd(window); wptr->ContentScaleCallback(*wptr,Point<float>{x,y});
}

void Window::dispatchMouseButtonCallback(GLFWwindow* window,int a,int b,int c){
	Window* wptr=mkwnd(window); wptr->MouseButtonCallback(*wptr,a,b,c);
}
void Window::dispatchCursorPosCallback(GLFWwindow* window,double x,double y){
	Window* wptr=mkwnd(window); wptr->CursorPosCallback(*wptr,Point<double>{x,y});
}
void Window::dispatchCursorEnterCallback(GLFWwindow* window,int e){
	Window* wptr=mkwnd(window); wptr->CursorEnterCallback(*wptr,e==GLFW_TRUE);
}
void Window::dispatchScrollCallback(GLFWwindow* window,double x,double y){
	Window* wptr=mkwnd(window); wptr->ScrollCallback(*wptr,Point<double>{x,y});
}
void Window::dispatchKeyCallback(GLFWwindow* window,int a,int b,int c,int d){
	Window* wptr=mkwnd(window); wptr->KeyCallback(*wptr,a,b,c,d);
}
void Window::dispatchCharCallback(GLFWwindow* window,unsigned int ch){
	Window* wptr=mkwnd(window); wptr->CharCallback(*wptr,ch);
}
//void Window::dispatchCharModsCallback(GLFWwindow*,unsigned int,int){} DEPRECATED
void Window::dispatchDropCallback(GLFWwindow* window,int count,const char* paths[]){
	std::vector<std::string> vpaths(paths,paths+count);
	Window* wptr=mkwnd(window); wptr->DropCallback(*wptr,vpaths);
}

Window& glfw::CurrentContext()
{
    return *mkwnd(glfwGetCurrentContext());
}
