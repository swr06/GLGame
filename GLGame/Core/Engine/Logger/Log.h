#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>

namespace GLGame
{
	namespace Log
	{
		using namespace std;

		void LogToConsole(string str);
		void LogToFile(string str);

		// Internally used functions

		static string ExtendString(string str, int ex_amt, string ex_c);
		string GetFileName(string path);
		void _InitLog(double ts, const char* file, int line_number);
		void _LogInfo(char* vendor, char* version, char* renderer);
		void _LogOpenGLInit(double ts, int version_minor, int version_major, const char* file, int line_number);
		void _LogWindowCreation(double ts, int width, int height, const char* file, int line_number);
		void _LogVSyncInit(double ts, bool vsync, const char* file, int line_number);
		void _LogTextureCreation(double ts, double elapsed_time, unsigned int tex_id, const string& tex_pth, const char* file, int line_number);
		//void _LogTextureLoading(double ts, uint32_t tex_id, const string& tex_pth, const char* file, int line_number);
		void _LogObjectCreation(double ts, double elapsed_time, const string& obj_id, bool visible, bool cull, bool has_sprite, const char* file, int line_number);
		void _LogTextureAtlasCreation(double ts, double elapsed_time, uint32_t tex_id, const string& tex_pth, const char* file, int line_number);
		void _LogSpriteCreation(double ts, double elapsed_time, const string& spr_id, const string& tex_pth, const char* file, int line_number);
		void _LogRendererInit(double ts, int max_size, const char* file, int line_number);
		void _LogEventInit(double ts, int call_back_count, const char* file, int line_number);
		void _LogShaderCreation(double ts, double elapsed_time, const char* vert_pth, const char* frag_pth, const char* file, int line_number);
		void _LogShaderError(double ts, const char* pth, const char* error, int err_type, const char* file, int line_number);
		void _Log();
	}
}