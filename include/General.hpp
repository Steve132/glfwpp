#ifndef GLFWPP_GENERAL_HPP
#define GLFWPP_GENERAL_HPP

#include "Include.hpp"
#include <stdexcept>
#include <string>
#include <vector>
#include <functional>


#define GLFWPP_ERROR 0x00020001

namespace glfw
{
	using Enum=int;
	class Exception:public std::runtime_error
	{
	public:
		Exception(int error_code,const std::string& description):std::runtime_error(description)
		{}
	};
	
	namespace impl {
		struct version_t {
			int major,minor,revision;
			std::string string; 
		};
	}
	extern impl::version_t version;
	
	namespace impl{
		void Verify();
	}
	namespace Init
	{
		void hint(Enum h,int value);
	}
	
	class Timer {
		static inline uint64_t Value() { return glfwGetTimerValue(); }
		static inline uint64_t Frequency() { return glfwGetTimerFrequency(); }
		operator double(){
			return glfwGetTime();
		}
		Timer& operator=(double d){ glfwSetTime(d); return *this; }
	};
	
	extern Timer Time;
	
	template<class T>
	struct Point{
		T x,y;
	};
	template<class T>
	struct Shape{
		T width,height;
	};
	struct ROI {
		int x,y,width,height;
	};
	
	struct Events{
		static void Poll(){
			glfwPollEvents();
		}
		static void Wait(){
			glfwWaitEvents();
		}
		static void WaitTimeout(double timeout){
			glfwWaitEventsTimeout(timeout);
		}
		static void PostEmpty() {
			glfwPostEmptyEvent();
		}
	};
	inline std::string KeyName(int key,int scancode){
		return glfwGetKeyName(key,scancode);
	}
#if GLFWPP_GLFW_VERSION_FULL >= 303
	inline bool RawMouseMotionSupported(){
		return glfwRawMouseMotionSupported()==GLFW_TRUE;
	}
	inline int KeyScancode(int key){
		return glfwGetKeyScancode(key);
	}
#endif
	inline void SwapInterval(int interval){
		glfwSwapInterval(interval);
	}
	inline int ExtensionSupported(const char* extension){
		return glfwExtensionSupported(extension)==GLFW_TRUE;
	}
	inline void* ProcAddress(const char* procname){
		return reinterpret_cast<void*>(glfwGetProcAddress(procname));
	}
	
#if GLFWPP_GLFW_VERSION_FULL > 303
	inline bool VulkanSuppported(){
		return glfwVulkanSupported()==GLFW_TRUE;
	}
#endif
	std::vector<std::string> RequiredInstanceExtensions();
	//TODO: Vulkan
	//InstanceProcAddress
	//PhysicalDevicePresentationSupport
	//
	
	extern std::function<void (int,const std::string&)> ErrorCallback;
}

#endif
