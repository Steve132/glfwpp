#ifndef GLFWPP_WINDOW_HPP
#define GLFWPP_WINDOW_HPP

#include "Include.hpp"
#include <memory>
#include <variant>
#include <unordered_map>
#include <string>
#include "Monitor.hpp"

namespace glfw
{
class Cursor;
class Window
{
protected:
	std::unique_ptr<GLFWwindow,void (*)(GLFWwindow*)> ptr;
	static void dispatchPosCallback(GLFWwindow* window, int,int);
	static void dispatchSizeCallback(GLFWwindow* window, int,int);
	static void dispatchCloseCallback(GLFWwindow* window);
	static void dispatchRefreshCallback(GLFWwindow* window);
	static void dispatchFocusCallback(GLFWwindow* window, int);
	static void dispatchIconifyCallback(GLFWwindow* window, int);
	static void dispatchMaximizeCallback(GLFWwindow* window, int);
	static void dispatchFramebufferSizeCallback(GLFWwindow* window, int,int);
	static void dispatchContentScaleCallback(GLFWwindow*,float,float);
	
	static void dispatchMouseButtonCallback(GLFWwindow*,int,int,int);
	static void dispatchCursorPosCallback(GLFWwindow*,double,double);
	static void dispatchCursorEnterCallback(GLFWwindow*,int);
	static void dispatchScrollCallback(GLFWwindow*,double,double);
	static void dispatchKeyCallback(GLFWwindow*,int,int,int,int);
	static void dispatchCharCallback(GLFWwindow*,unsigned int);
	//static void dispatchCharModsCallback(GLFWwindow*,unsigned int,int); Deprecated
	static void dispatchDropCallback(GLFWwindow*,int,const char*[]);
	Window(GLFWwindow*);
public:
	using Hint=std::variant<int,std::string>;
	using Hints=std::unordered_map<int,Hint>;
	Window(int width,int height,const Hints& hints=Hints(),const std::string& title=std::string(),const Monitor& fullscreen_mon=Monitor::None(),const Window* share=nullptr);
	//TODO: Vulkan with VkInstance
	
	GLFWwindow* pointer() { return ptr.get(); }

	void ShouldClose(bool doclose) {
		glfwSetWindowShouldClose(ptr.get(),doclose ? GLFW_TRUE : GLFW_FALSE);
	}
	bool ShouldClose() const {
		return glfwWindowShouldClose(ptr.get())==GLFW_TRUE;
	}
	operator bool() const { return !ShouldClose(); }
	bool operator==(const Window& o) const { return ptr==o.ptr; }
	bool operator!=(const Window& o) const { return ptr!=o.ptr; }	
	bool operator<(const Window& o) const { return ptr < o.ptr; }
	
	void Title(const std::string& title) {
		glfwSetWindowTitle(ptr.get(),title.c_str());
	}
	void Icon(const std::vector<GLFWimage>& images);
	
	Point<int> Pos() const {
		Point<int> p;
		glfwGetWindowPos(ptr.get(),&p.x,&p.y);
		return p;
	}
	void Pos(const Point<int>& p) {
		glfwSetWindowPos(ptr.get(),p.x,p.y);
	}
	Shape<int> Size() const {
		Shape<int> sz;
		glfwGetWindowSize(ptr.get(),&sz.width,&sz.height);
		return sz;
	}
	void Size(const Shape<int>& shp) {
		glfwSetWindowSize(ptr.get(),shp.width,shp.height);
	}
	void SizeLimits(const Shape<int>& mn,const Shape<int>& mx) {
		glfwSetWindowSizeLimits(ptr.get(),mn.width,mn.height,mx.width,mx.height);
	}
	void AspectRatio(int n,int d);
	void AspectRatio(const Shape<int>& shp) { 
		AspectRatio(shp.width,shp.height);
	}
	Shape<int> FramebufferSize() const {
		Shape<int> shp;
		glfwGetFramebufferSize(ptr.get(),&shp.width,&shp.height);
		return shp;
	}
	ROI FrameSize() const {
		ROI fbs;
		glfwGetWindowFrameSize(ptr.get(),&fbs.x,&fbs.y,&fbs.width,&fbs.height);
		fbs.width-=fbs.x;
		fbs.height-=fbs.y;
		return fbs;
	}
#if GLFWPP_GLFW_VERSION_FULL >= 303
	Point<float> ContentScale() const {
		Point<float> pnt;
		glfwGetWindowContentScale(ptr.get(),&pnt.x,&pnt.y);
		return pnt;
	}
	float Opacity() const {
		return glfwGetWindowOpacity(ptr.get());
	}
	void Opacity(float f) const {
		glfwSetWindowOpacity(ptr.get(),f);
	}
#endif

	void Iconify(){
		glfwIconifyWindow(ptr.get());
	}
	void Restore(){
		glfwRestoreWindow(ptr.get());
	}
	void Maximize(){
		glfwMaximizeWindow(ptr.get());
	}
	void Show(){
		glfwShowWindow(ptr.get());
	}
	void Hide(){
		glfwHideWindow(ptr.get());
	}
	void Focus(){
		glfwFocusWindow(ptr.get());
	}

	glfw::Monitor Monitor() const {
		return glfwGetWindowMonitor(ptr.get());
	}
	void Monitor(const Point<int>& corner,
				const Shape<int>& shp,
				const glfw::Monitor& newmon=glfw::Monitor::Primary(),
				int refreshRate = GLFW_DONT_CARE)
	{
		glfwSetWindowMonitor(ptr.get(), newmon.ptr, corner.x, corner.y, shp.width, shp.height, refreshRate);
	}
	int Attrib(int attr) const{
		return glfwGetWindowAttrib(ptr.get(),attr);
	}
#if GLFWPP_GLFW_VERSION_FULL >= 303
	void Attrib(int attr,int val) {
		glfwSetWindowAttrib(ptr.get(),attr,val);
	}
	void RequestAttention() {
		glfwRequestWindowAttention(ptr.get());
	}
#endif
	int InputMode(int mode) const{
		return glfwGetInputMode(ptr.get(),mode);
	}
	void InputMode(int mode,int value){
		glfwSetInputMode(ptr.get(),mode,value);
	}
	int Key(int key) const {
		return glfwGetKey(ptr.get(),key);
	}
	int MouseButton(int button) const {
		return glfwGetMouseButton(ptr.get(),button);
	}
	Point<double> CursorPos() const{
		Point<double> p;
		glfwGetCursorPos(ptr.get(),&p.x,&p.y);
		return p;
	}
	void CursorPos(const Point<double>& p) {
		glfwSetCursorPos(ptr.get(),p.x,p.y);
	}
	std::string ClipboardString() const{
		return glfwGetClipboardString(ptr.get());
	}
	void ClipboardString(const std::string& st){
		glfwSetClipboardString(ptr.get(),st.c_str());
	}
	void MakeContextCurrent() const{
		glfwMakeContextCurrent(ptr.get());
	}
	void SwapBuffers() {
		glfwSwapBuffers(ptr.get());
	}
	friend Window& CurrentContext();
	
	std::function<void (Window&,const Point<int>&)> PosCallback;
	std::function<void (Window&,const Shape<int>&)> SizeCallback;
	std::function<void (Window&)> CloseCallback;
	std::function<void (Window&)> RefreshCallback;
	std::function<void (Window&,bool)> FocusCallback;
	std::function<void (Window&,bool)> IconifyCallback;
	std::function<void (Window&,bool)> MaximizeCallback;
	std::function<void (Window&,const Shape<int>&)> FramebufferSizeCallback;
	std::function<void (Window&,const Point<float>&)> ContentScaleCallback;
	
	std::function<void (Window&,int,int,int)> MouseButtonCallback;
	std::function<void (Window&,const Point<double>&)> CursorPosCallback;
	std::function<void (Window&,bool)> CursorEnterCallback;
	std::function<void (Window&,const Point<double>&)> ScrollCallback;
	std::function<void (Window&,int,int,int,int)> KeyCallback;
	std::function<void (Window&,unsigned int)> CharCallback;
	std::function<void (Window&,const std::vector<std::string>&)> DropCallback;
	
	void Cursor(const glfw::Cursor& cur);
	//TODO: std::string->std::string_view
};
	Window& CurrentContext();
}

#endif
