#include "Log.h"

namespace GLGame
{
	// A basic logging system

	namespace Log
	{
		static const string log_header_text = "Tiny 2D OpenGL Game Engine [GLGame] v01 By Samuel Wesley Rasquinha (samuelrasquinha@gmail.com)\n\n\t--------------------------------------------LOG--------------------------------------------\n\n";
		static vector<string> log_queue;
		static const string log_filepath = (string)"log.txt";

		string ExtendString(string str, int ex_amt, string ex_c)
		{
			/*
			ExtendString() is used to extend a string to a particular size by adding a particular character to the beginning of a string
			Example : if ex_amt = 4, ex_c = '0', str = "9"
			then this function will return "0009"
			*/

			int temp = ex_amt - str.size();
			ex_amt = abs(temp);

			for (int i = 0; i < ex_amt; i++)
			{
				str.insert(0, ex_c);
			}

			return str;
		}

		string GetFileName(string path)
		{
			std::filesystem::path pth = std::filesystem::path(path.c_str()).filename();

			return pth.string();
		}

		void _InitLog(double ts, const char* file, int line_number)
		{
			stringstream s;
			stringstream info;

			s << "\nTIME : [" << ts << "]\tTYPE : INIT_LOG   ";

			log_queue.insert(log_queue.begin(), log_header_text);
		}

		void _LogInfo(char* vendor, char* version, char* renderer)
		{
			stringstream info;

			info << "VENDOR : " << vendor << "\n" << "GL VERSION : " << version << "\n" << "RENDERER : " << renderer << "\n............................................\n";
			log_queue.insert(log_queue.begin() + 1, info.str());
		}

		void _LogOpenGLInit(double ts, int version_minor, int version_major, const char* file, int line_number)
		{
			stringstream s;

			s << "\nTIME : [" << ts << "]\tTYPE : INIT_GL " << version_minor << "." << version_major
				<< "\t| FILE : " << GetFileName(file) << "\tLINE : ["
				<< line_number << "]\n";

			log_queue.push_back(s.str());
		}

		void _LogWindowCreation(double ts, int width, int height, const char* file, int line_number)
		{
			stringstream s;

			s << "\nTIME : [" << ts << "]\tTYPE : INIT_WINDOW | WIDTH : " << width << " HEIGHT : " << height
				<< "\t| FILE : " << GetFileName(file) << "\tLINE : ["
				<< line_number << "]  \n";

			log_queue.push_back(s.str());
		}

		void _LogVSyncInit(double ts, bool vsync, const char* file, int line_number)
		{
			stringstream s;
			string _vsync = (string)"false";

			if (vsync)
			{
				_vsync = (string)"true";
			}

			s << "\nTIME : [" << ts << "]\tTYPE : INIT_VSYNC : " << _vsync
				<< "\t| FILE : " << GetFileName(file) << "\tLINE : ["
				<< line_number << "]\t\n";

			log_queue.push_back(s.str());
		}

		void _LogTextureCreation(double ts, double elapsed_time, unsigned int tex_id, const string& tex_pth, const char* file, int line_number)
		{
			stringstream s;
			string _vsync = (string)"false";

			s << "\nTIME : [" << ts << "]\tELAPSED TIME : " << elapsed_time << "\tTYPE : TEXTURE_CREATION [" << tex_pth << "] ID : [" << tex_id
				<< "]\t| FILE : " << GetFileName(file) << "\tLINE : ["
				<< line_number << "]\t\n";

			log_queue.push_back(s.str());
		}

		/*void _LogTextureLoading(double ts, uint32_t tex_id, const string& tex_pth, const char* file, int line_number)
		{
			stringstream s;

			s << "\nTIME : [" << ts << "]\tTYPE : TEXTURE_LOADING [" << tex_pth << "] ID : [" << tex_id
				<< "]\t| FILE : " << GetFileName(file) << "\tLINE : ["
				<< line_number << "]\t\n";

			log_queue.push_back(s.str());
		}*/

		void _LogObjectCreation(double ts, double elapsed_time, const string& obj_id, bool visible, bool cull, bool has_sprite, const char* file, int line_number)
		{
			stringstream s;
			string _vsync = (string)"false";

			s << "\nTIME : [" << ts << "]\tTYPE : CREATE_OBJECT [" << obj_id << "]" << "\tELAPSED_TIME : " << elapsed_time << "\n\tVISIBLE : " << visible 
				<< "\n\tHAS_SPRITE : " << has_sprite << "\n\tCULL : " << cull << "\t| FILE : " << GetFileName(file) << "\tLINE : ["
				<< line_number << "]\t\n";

			log_queue.push_back(s.str());
		}

		void _LogTextureAtlasCreation(double ts, double elapsed_time, uint32_t tex_id, const string& tex_pth, const char* file, int line_number)
		{
			stringstream s;

			s << "\nTIME : [" << ts << "]\tTYPE : TEXTURE_ATLAS_CREATION\tELAPSED_TIME : " << elapsed_time << "\t[" << tex_pth << "] ID : [" << tex_id
				<< "]\t| FILE : " << GetFileName(file) << "\tLINE : ["
				<< line_number << "]\t\n";

			log_queue.push_back(s.str());
		}

		void _LogSpriteCreation(double ts, double elapsed_time, const string& spr_id, const string& tex_pth, const char* file, int line_number)
		{
			stringstream s;

			s << "\nTIME : [" << ts << "]\tTYPE : CREATE_SPRITE\tELAPSED_TIME : " << elapsed_time <<"\tPTH [" 
				<< tex_pth << "] ID : [" << spr_id << "]\t| FILE : " << GetFileName(file)
				<< "\tLINE : [" << line_number << "]\t\n";

			log_queue.push_back(s.str());
		}

		void _LogRendererInit(double ts, int max_size, const char* file, int line_number)
		{
			stringstream s;

			s << "\nTIME : [" << ts << "]\tTYPE : INIT_RENDERER\t" << "MAX OBJECTS : " << max_size 				<< "\t| FILE : " << GetFileName(file) << "\tLINE : ["
				<< line_number << "]\n";

			log_queue.push_back(s.str());
		}

		void _LogEventInit(double ts, int call_back_count, const char* file, int line_number)
		{
			stringstream s;

			s << "\nTIME : [" << ts << "]\tTYPE : EVENT_INIT\t" << "CALLBACKS SET : " << call_back_count << "\t| FILE : " << GetFileName(file) << "\tLINE : ["
				<< line_number << "]\n";

			log_queue.push_back(s.str());
		}

		void _LogShaderCreation(double ts, double elapsed_time, const char* vert_pth, const char* frag_pth, const char* file, int line_number)
		{
			stringstream s;

			s << "\nTIME : [" << ts << "]\tTYPE : CREATE_SHADER\tELAPSED TIME : " << elapsed_time<< "\n\tVERTEX PTH : " << vert_pth << "\n\tFRAGMENT PTH : " << frag_pth << "\n\t| FILE : " << GetFileName(file) << "\tLINE : ["
				<< line_number << "]\n";

			log_queue.push_back(s.str());
		}

		void _LogShaderError(double ts, const char* pth, const char* error, int err_type, const char* file, int line_number)
		{
			stringstream s;

			if (err_type == 0)
			{
				s << "\nTIME : [" << ts << "]\tTYPE : VERTEX_SHADER_ERROR\t" << "\n\tVERTEX_SHADER PTH : " << pth
					<< "\n\n\t---------------------------GL INFO LOG---------------------------" << "\n\t" << error
					<< "\n\t| FILE : " << GetFileName(file) << "\tLINE : ["
					<< line_number << "]\n";
			}

			else if (err_type == 1)
			{
				s << "\nTIME : [" << ts << "]\tTYPE : FRAGMENT_SHADER_ERROR\t" << "\n\tFRAGMENT_SHADER PTH : " << pth 
					<< "\n\n\t---------------------------GL INFO LOG---------------------------" << "\n\t" << error 
					<< "\n\t| FILE : " << GetFileName(file) << "\tLINE : ["
					<< line_number << "]\n";
			}
			
			else
			{
				s << "\nTIME : [" << ts << "]\tTYPE : UNDEFINED_SHADER_ERROR\t" << "\n\tUNDEFINED_SHADER PTH.." << "\n\t| FILE : " << GetFileName(file) << "\tLINE : ["
					<< line_number << "]\n";
			}

			log_queue.push_back(s.str());
		}

		void _Log() 
		{
			 static bool first_run = true;
			 int logs_written = 0;
			 ofstream log_file;

			 if (first_run)
			 {
				 log_file.open(log_filepath, ios::out | ios::trunc);
			 }

			 else
			 {
				 log_file.open(log_filepath, ios::out | ios::app);
			 }

			 if (log_file.good() && log_file.is_open())
			 {
				 log_file.seekp(0, ios::end);

				 for (int i = 0; i < log_queue.size(); i++)
				 {
					 log_file.write(log_queue[i].c_str(), log_queue[i].size());
					 logs_written++;
				 }

				 log_queue.erase(log_queue.begin(), log_queue.begin() + logs_written);
			 }

			 else
			 {
				 LogToConsole((string)"Unable to open Log file to write the log..."); 
			 }

			 first_run = false;
			 log_file.close();
		}

		void LogToConsole(string str)
		{
			std::cout << "\nLOG : " << str << endl;
		}

		void LogToFile(string str)
		{
			log_queue.push_back(str);
		}
	}
}