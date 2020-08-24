#include "General.hpp"
using namespace glfw;


bool is_glfw_initialized=false; 


namespace Init
{
	#if GLFWPP_GLFW_VERSION_FULL >= 303 
	void hint(int h,int v)
	{
		if(is_glfw_initialized) throw Exception(GLFWHPP_ERROR,"Init Hint cannot be given after initialization");
		glfwInitHint(h,v);
	}
	#else
	void hint(int h,int v)
	{
		if(is_glfw_initialized) throw Exception(GLFWHPP_ERROR,"Init Hint cannot be given after initialization");
	}
	#endif
}
Timer glfw::Time;
struct GLFWInit
{
public:
	GLFWInit(){
		glfwInit();
		is_glfw_initialized=true;
	}
	~GLFWInit()
	{
		glfwTerminate();
	}
	static GLFWInit& Verify()
	{
		static GLFWInit glfwi;
		return glfwi;
	}
};

static impl::version_t getVersion()
{
	impl::version_t vers;
	glfwGetVersion(&vers.major,&vers.minor,&vers.revision);
	vers.string=glfwGetVersionString();
	return vers;
}
impl::version_t version=getVersion();
