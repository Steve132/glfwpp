#ifndef GLFWPP_MONITOR_HPP
#define GLFWPP_MONITOR_HPP

#include "Include.hpp"
#include "General.hpp"
#include <functional>
#include <unordered_map>
#include <vector>
#include <array>
#include <iosfwd>

namespace glfw
{
	class Window;
	class Monitor
	{
	protected:
		GLFWmonitor* ptr;
		friend class Window;
		using connected_callback_type=std::function<void (const Monitor& m,bool connected)>;
		
		static void callbackDispatch(GLFWmonitor* m,int connected);
		static std::unordered_map<GLFWmonitor*,connected_callback_type> cbmap;

	public:
		static std::vector<Monitor> Monitors();
		static Monitor Primary();
		static Monitor None();
		
		bool operator==(const Monitor& o) const { return ptr==o.ptr; }
		bool operator!=(const Monitor& o) const { return ptr!=o.ptr; }	
		bool operator<(const Monitor& o) const { return ptr < o.ptr; }
		
		connected_callback_type& Callback;
		
		GLFWvidmode VideoMode() const;
		std::vector<GLFWvidmode> VideoModes() const;
		
		Point<int> Pos() const {
			Point<int> p;
			glfwGetMonitorPos(ptr,&p.x,&p.y);
			return p;
		}
				
#if GLFWPP_GLFW_VERSION_FULL >= 303
		ROI Workarea() const{
			ROI ar;
			glfwGetMonitorWorkarea(ptr,&ar.x,&ar.y,&ar.width,&ar.height);
			return ar;
		}
		Point<float> ContentScale() const {
			Point<float> pf;
			glfwGetMonitorContentScale(ptr,&pf.x,&pf.y);
			return pf;
		}
#endif
		Shape<int> PhysicalSize() const {
			Shape<int> sp;
			glfwGetMonitorPhysicalSize(ptr,&sp.width,&sp.height);
			return sp;
		}
		std::string Name() const {
			return glfwGetMonitorName(ptr);
		}
		
		void Gamma(float fg)
		{
			glfwSetGamma(ptr,fg);
		}
		
		struct Ramp {
			std::vector<uint16_t> red,green,blue;
		};
		
		void Gamma(const Ramp& rmp);
		Ramp Gamma() const;
	protected:
		Monitor(GLFWmonitor* tptr):ptr(tptr),Callback(cbmap[tptr])
		{}
	};
}

std::ostream& operator<<(std::ostream&,const GLFWvidmode&);
std::istream& operator>>(std::istream&,GLFWvidmode&);

#endif
