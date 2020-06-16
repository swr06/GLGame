#include "SceneFileParser.h"

// A GLGame Scene file parser

namespace GLGame
{
	namespace SceneParser
	{
		static const string object_chunk_type = (string)"OBJ";
		static const string sprite_chunk_type = (string)"SPR"; 
		static const string atlas_chunk_type = (string)"ATL";
		static const string background_chunk_type = (string)"BG_";

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
			Sprite* spr;
			Background* bg;

			glm::vec3 pos;

			for (int i = 0; i < items.size(); i++)
			{
				if (items[i].type == ItemTypeObject)
				{
					obj = (Object*)GameInternal::_GetObjectFromGlobalArray(items[i].id);

					if (obj != nullptr)
					{
						pos.x = items[i].x;
						pos.y = items[i].y;

						// If the object has a sprite, correct the x and y according to the width and height
						// x = x - (w/2)
						// y = y - (h/2)

						if (obj->HasSprite())
						{
							pos.x = pos.x - (int)(obj->GetSprite()->GetCurrentTextureWidth() / 2);
							pos.y = pos.y - (int)(obj->GetSprite()->GetCurrentTextureHeight() / 2);
						}

						pos.z = 1.0f;
						scene->AddObjectAtPosition(*obj, items[i].layer, pos);
					}
				}

				else if (items[i].type == ItemTypeSprite)
				{
					spr = (Sprite*)GameInternal::_GetSpriteFromGlobalArray(items[i].id);

					if (spr != nullptr)
					{
						pos.x = items[i].x;
						pos.y = items[i].y;
						pos.z = 1.0f;
						scene->AddSpriteAtPosition(*spr, items[i].layer, pos);
					}
				}

				else if (items[i].type == ItemTypeBackground)
				{
					bg = (Background*)GameInternal::_GetBackgroundFromGlobalArray(items[i].id);

					if (bg != nullptr)
					{
						scene->AddSceneBackground(bg, items[i].layer);
					}
				}
			}
		}

		void RemoveCharacterFromArray(char* character_array, char character, size_t size) // This overload is only to be used by the ExtractFile() header and implementation
		{
			string str = string(character_array);

			for (int i = 0; i < size; i++)
			{
				str.erase(std::find(str.begin(), str.end(), character));
			}

			for (int i = 0; i < str.size(); i++)
			{
				character_array[i] = str[i];
			}

			character_array[str.size()] = '\0';
		}

		void RemoveCharacterFromString(string& str, char character) // This overload is only to be used by the ExtractFile() header and implementation
		{
			for (int i = 0; i < str.size(); i++)
			{
				str.erase(std::find(str.begin(), str.end(), character));
			}
		}

		// Function to parse the inputted scene file data
		void _ParseSceneFileData(Scene* scene, const string& scene_file)
		{
			SceneParsedDataItem item;
			vector<SceneParsedDataItem> scene_parsed_items;

			ifstream scene_data_file;
			const string scene_header_text = string(GLGAME_SCENE_FILE_HEADER);
			const int buffer_size = 256;
			const int min_file_size = (scene_header_text.size() * sizeof(char)) + 20;
			int chunk_type = 0;

			int scene_data_curr_idsz = 0;
			const short default_schunk_size = 8; // default small chunk size

			char* scene_data_curr_type = new char[8];
			char* scene_header_buff = new char[scene_header_text.size() + 1];
			char* scene_data_curr_id = new char[512];
			char* scene_data_curr_x = new char[16];
			char* scene_data_curr_y = new char[16];
			char* scene_data_curr_sx = new char[16];
			char* scene_data_curr_sy = new char[16];
			char* scene_data_curr_layer = new char[16];
			char* scene_data_curr_idsz_buff = new char[16];
			char* scene_dada_curr_garbage_buff = new char[16];
			

			// Fill all the character buffers with NULL
			memset(scene_header_buff, '\0', scene_header_text.size() + 1);
			memset(scene_data_curr_id, '\0', 512);
			memset(scene_data_curr_type, '\0', 8);
			memset(scene_data_curr_x, '\0', 16);
			memset(scene_data_curr_y, '\0', 16);
			memset(scene_data_curr_sx, '\0', 16);
			memset(scene_data_curr_sy, '\0', 16);
			memset(scene_data_curr_layer, '\0', 16);
			memset(scene_data_curr_idsz_buff, '\0', 16);
			memset(scene_dada_curr_garbage_buff, '\0', 16);

			scene_data_file.open(scene_file, ios::in);

			if (scene_data_file.is_open() && scene_data_file.good())
			{
				// Checking if the file is valid
				scene_data_file.read(scene_header_buff, 23);

				if (!strcmp(scene_header_buff, scene_header_text.c_str()))
				{
					while (!scene_data_file.eof())
					{
						scene_data_file.read(scene_data_curr_type, 3);

						if (!strcmp(scene_data_curr_type, object_chunk_type.c_str()))
						{
							// read format = object chunk
							scene_data_file.read(scene_data_curr_layer, 8);
							scene_data_file.read(scene_data_curr_x, 12);
							scene_data_file.read(scene_data_curr_y, 12);

							scene_data_file.read(scene_data_curr_idsz_buff, 8);
							RemoveCharacterFromArray(scene_data_curr_idsz_buff, '$', 8);
							int rd_size = atoi(scene_data_curr_idsz_buff);
							scene_data_file.read(scene_data_curr_id, rd_size);
							scene_data_curr_id[rd_size] = '\0';

							RemoveCharacterFromArray(scene_data_curr_layer, '!', 8);
							RemoveCharacterFromArray(scene_data_curr_x, '@', 12);
							RemoveCharacterFromArray(scene_data_curr_y, '#', 12);
		
							// converting the values to a struct
							item.type = ItemTypeObject;
							item.id = string(scene_data_curr_id);
							item.x = (float)atoi(scene_data_curr_x);
							item.y = (float)atoi(scene_data_curr_y);
							item.layer = atoi(scene_data_curr_layer);

							// reading the garbage string
							scene_data_file.read(scene_dada_curr_garbage_buff, 8);

							// adding it to the vector
							scene_parsed_items.push_back(item);
						}

						else if (!strcmp(scene_data_curr_type, sprite_chunk_type.c_str()))
						{
							// read format = sprite chunk
							
							scene_data_file.read(scene_data_curr_layer, 8);
							scene_data_file.read(scene_data_curr_x, 12);
							scene_data_file.read(scene_data_curr_y, 12);

							scene_data_file.read(scene_data_curr_idsz_buff, 8);
							RemoveCharacterFromArray(scene_data_curr_idsz_buff, '$', 8);
							int rd_size = atoi(scene_data_curr_idsz_buff);
							scene_data_file.read(scene_data_curr_id, rd_size);
							scene_data_curr_id[rd_size] = '\0';

							RemoveCharacterFromArray(scene_data_curr_layer, '!', 8);
							RemoveCharacterFromArray(scene_data_curr_x, '@', 12);
							RemoveCharacterFromArray(scene_data_curr_y, '#', 12);

							// converting the values to a struct
							item.type = ItemTypeSprite;
							item.id = string(scene_data_curr_id);
							item.x = (float)atoi(scene_data_curr_x);
							item.y = (float)atoi(scene_data_curr_y);
							item.layer = atoi(scene_data_curr_layer);

							// reading the garbage string
							scene_data_file.read(scene_dada_curr_garbage_buff, 8);

							// adding it to the vector
							scene_parsed_items.push_back(item);
						}

						else if (!strcmp(scene_data_curr_type, background_chunk_type.c_str()))
						{
							// read format = sprite chunk

							scene_data_file.read(scene_data_curr_layer, 8);
							scene_data_file.read(scene_data_curr_x, 12);
							scene_data_file.read(scene_data_curr_y, 12);

							scene_data_file.read(scene_data_curr_idsz_buff, 8);
							RemoveCharacterFromArray(scene_data_curr_idsz_buff, '$', 8);
							int rd_size = atoi(scene_data_curr_idsz_buff);
							scene_data_file.read(scene_data_curr_id, rd_size);
							scene_data_curr_id[rd_size] = '\0';

							RemoveCharacterFromArray(scene_data_curr_layer, '!', 8);
							RemoveCharacterFromArray(scene_data_curr_x, '@', 12);
							RemoveCharacterFromArray(scene_data_curr_y, '#', 12);

							// converting the values to a struct
							item.type = ItemTypeBackground;
							item.id = string(scene_data_curr_id);
							item.x = (float)atoi(scene_data_curr_x);
							item.y = (float)atoi(scene_data_curr_y);
							item.layer = atoi(scene_data_curr_layer);

							// reading the garbage string
							scene_data_file.read(scene_dada_curr_garbage_buff, 8);

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
					Log::LogToConsole(s.str());
				}
			}

			else
			{
				stringstream s;

				s << "ERROR : The scene file path " << scene_file << " is invalid !";

				Log::LogToFile(s.str());
				Log::LogToConsole(s.str());
			}


			delete[] scene_data_curr_type;
			delete[] scene_header_buff;
			delete[] scene_data_curr_id;
			delete[] scene_data_curr_x;
			delete[] scene_data_curr_y;
			delete[] scene_data_curr_sx;
			delete[] scene_data_curr_sy;
			delete[] scene_data_curr_layer;
			delete[] scene_data_curr_idsz_buff;
			delete[] scene_dada_curr_garbage_buff;
		}
	}
}