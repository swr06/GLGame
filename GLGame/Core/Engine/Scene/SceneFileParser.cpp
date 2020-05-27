#include "SceneFileParser.h"

// Highly W.I.P

namespace GLGame
{
	namespace SceneParser
	{
		static const string object_chunk_type = (string)"001";
		static const string sprite_chunk_type = (string)"002";
		static const string atlas_chunk_type = (string)"003";

		std::ifstream::pos_type GetFileSize(const string& filename)
		{
			std::ifstream in(filename.c_str(), std::ifstream::ate | std::ifstream::binary);
			std::ifstream::pos_type ret = in.tellg();
			in.close();

			return ret;
		}

		void ParseSceneData(const string& scene_file_pth, Scene* scene)
		{
			_ParseSceneFileData(scene, scene_file_pth);
		}

		// This function will fill in the objects that were parsed
		void _FillInParsedSceneData(vector<SceneParsedDataItem>& items, Scene* scene)
		{
			Object* obj;
			glm::vec3 pos;

			for (int i = 0; i < items.size(); i++)
			{
				if (items[i].type == ItemTypeObject)
				{
					obj = (Object*) GameInternal::_GetObjectFromGlobalArray(items[i].id);
					pos.x = items[i].x;
					pos.y = items[i].y;
					pos.z = items[i].z;
					scene->AddObjectAtPosition(*obj, items[i].layer, pos);
				}
			}
		}

		// Function to parse the inputted scene file data
		void _ParseSceneFileData(Scene* scene, const string& scene_file)
		{
			SceneParsedDataItem item;
			vector<SceneParsedDataItem> scene_parsed_items;
			ifstream::pos_type scene_file_size = GetFileSize(scene_file);

			ifstream scene_data_file;
			const string scene_header_text = string(GLGAME_SCENE_FILE_HEADER);
			const int buffer_size = 256;
			const int min_file_size = (scene_header_text.size() * sizeof(char)) + 20;
			int chunk_type = 0;
			char* scene_data_buff = new char[buffer_size];

			int scene_data_curr_idsz = 0;
			const short default_schunk_size = 8; // default small chunk size

			char* scene_data_curr_id = new char[512];
			char* scene_data_curr_x = new char[default_schunk_size];
			char* scene_data_curr_y = new char[default_schunk_size];
			char* scene_data_curr_z = new char[default_schunk_size];
			char* scene_data_curr_sx = new char[default_schunk_size];
			char* scene_data_curr_sy = new char[default_schunk_size];
			char* scene_data_curr_layer = new char[default_schunk_size];
			char* scene_data_curr_idsz_buff = new char[6]; 

			// Fill all the character buffers with NULL
			memset(scene_data_buff, '\0', buffer_size);
			memset(scene_data_curr_id, '\0', 512);
			memset(scene_data_curr_x, '\0', default_schunk_size);
			memset(scene_data_curr_y, '\0', default_schunk_size);
			memset(scene_data_curr_z, '\0', default_schunk_size);
			memset(scene_data_curr_sx, '\0', default_schunk_size);
			memset(scene_data_curr_sy, '\0', default_schunk_size);
			memset(scene_data_curr_layer, '\0', default_schunk_size);
			memset(scene_data_curr_idsz_buff, '\0', 6);

			scene_data_file.open(scene_file, ios::in);
			scene_data_file.seekg(0, ios::end);

			if (scene_data_file.is_open() && scene_data_file.good())
			{
				// Checking if the file is valid
				scene_data_file.read(scene_data_buff, scene_header_text.size());

				if (!strcmp(scene_data_buff, scene_header_text.c_str()))
				{
					while (!scene_data_file.eof())
					{
						scene_data_file.read(scene_data_buff, 3);
						scene_data_file.read(scene_data_curr_idsz_buff, 4);
						scene_data_curr_idsz = atoi(scene_data_curr_idsz_buff);

						if (!strcmp(scene_data_buff, object_chunk_type.c_str()))
						{
							// read format = object chunk
							scene_data_file.read(scene_data_curr_id, scene_data_curr_idsz);
							scene_data_file.read(scene_data_curr_x, 5);
							scene_data_file.read(scene_data_curr_y, 5);
							scene_data_file.read(scene_data_curr_z, 5);
							scene_data_file.read(scene_data_curr_layer, 4);

							// converting the values to a struct
							item.type = ItemTypeObject;
							item.id =scene_data_curr_id;
							item.x = (uint32_t)atoi(scene_data_curr_x);
							item.y = (uint32_t)atoi(scene_data_curr_y);
							item.z = (uint32_t)atoi(scene_data_curr_z);
							item.layer = (uint32_t)atoi(scene_data_curr_layer);

							// adding it to the vector
							scene_parsed_items.push_back(item);
						}

						else if (!strcmp(scene_data_buff, sprite_chunk_type.c_str()))
						{
							// read format = object chunk

							// read format = object chunk
							scene_data_file.read(scene_data_curr_id, scene_data_curr_idsz);
							scene_data_file.read(scene_data_curr_x, 5);
							scene_data_file.read(scene_data_curr_y, 5);
							scene_data_file.read(scene_data_curr_z, 5);
							scene_data_file.read(scene_data_curr_layer, 4);

							// converting the values to a struct
							item.type = ItemTypeSprite;
							item.id = scene_data_curr_id;
							item.x = (uint32_t)atoi(scene_data_curr_x);
							item.y = (uint32_t)atoi(scene_data_curr_y);
							item.z = (uint32_t)atoi(scene_data_curr_z);
							item.layer = (uint32_t)atoi(scene_data_curr_layer);

							// adding it to the vector
							scene_parsed_items.push_back(item);
						}
					}

					_FillInParsedSceneData(scene_parsed_items, scene);
				}

				else
				{
					stringstream s;
					s << "ERROR : The scene file " << scene_file << " is not a valid GLGAME scene file.";

					Log::LogToFile(s.str());
				}
			}
		}
	}
}