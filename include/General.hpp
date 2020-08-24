#ifndef GLFWPP_GENERAL_HPP
#define GLFWPP_GENERAL_HPP

#include "Include.hpp"
#include <stdexcept>
#include <string>
#define GLFWPP_ERROR 0x00020001

namespace glfw
{
	using Enum=int;
	class Exception:public std::runtime_error
	{
	public:
		Exception(int error_code,const std::string& description);
	};
	
	namespace impl {
		struct version_t {
			int major,minor,revision;
			std::string string; 
		};
	}
	extern impl::version_t version;
	
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
}

#endif
