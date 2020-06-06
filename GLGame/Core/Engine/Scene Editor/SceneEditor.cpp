#include "SceneEditor.h"

// The GLGame Scene editor has to be called at runtime. 

namespace GLGame
{
	namespace SceneEditor
	{
		enum SceneEditorItemTypes
		{
			SE_ObjectType,
			SE_SpriteType,
		};

		struct SceneEditorRenderItem
		{
			Object* obj = nullptr;
			Sprite* spr = nullptr;
			float x;
			float y;
			int layer;
			SceneEditorItemTypes item_type;
		};

		static enum CurrentItemTypeSelection
		{
			ObjectSelection = 0,
			SpriteSelection,
			Nothing
		};

		static enum Operations
		{
			PlaceItems = 0,
			ChangeBackground,
			SeeProperties,
			DebugGame,
			ViewScene,
		};

		// Variables to initialize the window
		static bool SceneEditorInitialized = false;
		static GLFWwindow* SceneEditorWindow;
		static int SceneEditorWidth = 1366;
		static int SceneEditorHeight = 750;
		static bool SE_window_destroyed = false;

		// Scene Editor event variables
		static double StartPanX = 0;
		static double StartPanY = 0;

		// Data to display/store/show
		static unordered_map<string, Object*>* SceneEditorGlobalObjects = nullptr;
		static unordered_map<string, Sprite*>* SceneEditorGlobalSprites = nullptr;
		static vector<string>* ObjectIDList = nullptr;
		static vector<string>* SpriteIDList = nullptr;
		static SpriteBatcher* SceneEditorBatcher;

		// Data used by the UI elements and widgets

		// To select the type of data
		static int RadioObjectSelected = -1;
		static int RadioSpriteSelected = -1;
		static int ItemTypeSelected = Nothing;

		// Render queue and layering
		static float CurrentSceneEditorLayer = 0;
		static map<int, vector<SceneEditorRenderItem>> SceneEditorItemQueue;
		static SceneEditorRenderItem GhostObjectImage;

		static Shader SceneEditorRenderItemShader;

		// Saving file

		static char* SceneFilePath = nullptr;
		static bool SceneFilePathSet = false;
		static const size_t SceneFilePthSize = 512;
		static bool UnsavedChanges = true;

		// Mouse and window attributes
		static double MousePosX = 0;
		static double MousePosY = 0;
		static int WindowSizeX = 0;
		static int WindowSizeY = 0;
		static int GridX = 64;
		static int GridY = 64;
		static double MouseScrollOffsetX = 0;
		static double MouseScrollOffsetY = 0;

		static const char* ObjectInsertString = "@#$*#\0";
		static const char* SpriteInsertString = "$*$^(\0";

		// Modal windows

		// Close modal window
		static bool ShouldShowCloseModalWindow = false;

		static bool ShouldShowAboutTheAuthorWindow = false;
		static bool ShouldShowSupportMeWindow = false;
		static bool ShouldShowWITWindow = false; // What is this window
		static bool ShouldShowDependenciesWindow = false;

		// Saving modal window
		static bool ShouldShowSaveAsWindow = false;
		static bool ShouldShowFileErrWindow = false;

		// Input blocking
		static bool ShouldBlockInput = false;

		// IMGUI context => Scene editor window
		ImGuiContext* imcontext;

		// Scene editor matrices
		Camera* SceneEditorCamera;

		// Other needed structures
		int CurrentOperationSelected = 0; // Has to be an int for imgui. Used as an enum class "Operations" 

		// Debug Window
		GameDebugInfo* DebugInfo;

		///////////////////////
		GLFWwindow* _Init(GLFWwindow* share_window, ImGuiContext* context);
		void ExtendString(string& str, int ex_amt, const string& ex_c);
		void FlushSceneFile();
		void _ShowModalWindows();
		void _RenderSceneEditorItems();
		void _DrawSEWidgets();
		void _DrawSEPlaceItemWidgets();
		void _DrawSEMenuBar();
		void _SetSceneEditorCloseFlag(bool val);
		void SEWindowResizeCallback(GLFWwindow* window, int width, int height);
		float SnapToGrid(int value, int size);
		void SEKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void SECursorPosCallback(GLFWwindow* window, double xpos, double ypos);
		void SEMouseCallback(GLFWwindow* window, int button, int action, int mods);
		void SEScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		void SEWindowCloseCallback(GLFWwindow* window);
		void _DrawGrid();
		void _DrawDebugWindow();
		///////////////////////

		GLFWwindow* InitSceneEditor(GameDebugInfo* debug_info, unordered_map<string, Object*>* global_objects, unordered_map<string, Sprite*>* global_sprites, vector<string>* objid_list, vector<string>* sprid_list, GLFWwindow* window, ImGuiContext* context)
		{
			DebugInfo = debug_info;
			SceneEditorGlobalObjects = global_objects;
			SceneEditorGlobalSprites = global_sprites;
			ObjectIDList = objid_list;
			SpriteIDList = sprid_list;

			return (_Init(window, context));
		}

		GLFWwindow* _Init(GLFWwindow* share_window, ImGuiContext* context)
		{
			glfwInit();

			SceneEditorWindow = glfwCreateWindow(SceneEditorWidth, SceneEditorHeight, "GLGame Scene Editor", nullptr, share_window);

			if (SceneEditorWindow == 0)
			{
				Log::LogToConsole("Scene Editor window could not be created!");
				return nullptr;
			}

			glfwMakeContextCurrent(SceneEditorWindow);

			// Set all the GLFW Event Callbacks required

			//glfwSetCursorPosCallback(m_GameWindow, GLGameCursorPosCallback);
			glfwSetKeyCallback(SceneEditorWindow, SEKeyCallback);
			glfwSetMouseButtonCallback(SceneEditorWindow, SEMouseCallback);
			glfwSetScrollCallback(SceneEditorWindow, SEScrollCallback);
			glfwSetFramebufferSizeCallback(SceneEditorWindow, SEWindowResizeCallback);
			glfwSetWindowCloseCallback(SceneEditorWindow, SEWindowCloseCallback);
			glfwSetCursorPosCallback(SceneEditorWindow, SECursorPosCallback);

			SceneEditorInitialized = true;

			// Imgui initialization

			ImGui::SetCurrentContext(ImGui::CreateContext());
			imcontext = ImGui::GetCurrentContext();
			ImGui_ImplGlfw_InitForOpenGL(SceneEditorWindow, true);
			ImGui_ImplOpenGL3_Init((const char*)"#version 130");
			ImGui::StyleColorsDark();

			// Build the font atlas
			ImGuiIO& io = ImGui::GetIO();
			io.Fonts->AddFontDefault();
			io.Fonts->Build();

			// Imgui style
			ImGuiStyle& style = ImGui::GetStyle();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

			// Set up character buffers
			SceneFilePath = new char[SceneFilePthSize];
			memset(SceneFilePath, '\0', SceneFilePthSize);

			// Setup camera, batchers and shaders
			SceneEditorCamera = new Camera(0.0f, (float)SceneEditorWidth, 0.0f, (float)SceneEditorHeight);
			SceneEditorBatcher = new SpriteBatcher();
			SceneEditorRenderItemShader.CreateShaderProgram(GLGAME_DEFAULT_SE_VERTEX, GLGAME_DEFAULT_SE_FRAGMENT);
			return SceneEditorWindow;
		}

		void OpenSceneFile()
		{

		}

		void ExtendString(string& str, int ex_amt, const string& ex_c)
		{
			int temp = ex_amt - str.size();
			ex_amt = abs(temp);

			for (int i = 0; i < ex_amt; i++)
			{
				str.insert(0, ex_c);
			}
		}

		void FlushSceneFile()
		{
			static string scene_file_header = string(GLGAME_SCENE_FILE_HEADER);
			char* scene_garbage_str = new char[12];
			fstream scene_file;

			// item write strings
			string item_type_str;
			string layer;
			string x, y;
			string id;
			string id_size;

			memset(scene_garbage_str, '\0', 12);
			scene_file.open(string(SceneFilePath), ios::out | ios::binary);

			if (scene_file.good() && scene_file.is_open())
			{
				// Write the scene header
				scene_file.write(scene_file_header.c_str(), scene_file_header.size());

				for (auto e = SceneEditorItemQueue.begin(); e != SceneEditorItemQueue.end(); e++)
				{
					for (int i = 0; i < e->second.size(); i++)
					{
						SceneEditorRenderItem item;

						item = e->second.at(i);
						layer = to_string(item.layer);
						x = to_string((int)item.x);
						y = to_string((int)item.y);

						if (item.item_type == SE_ObjectType)
						{
							item_type_str = "OBJ";
							id = item.obj->GetObjectID();
						}

						else if (item.item_type == SE_SpriteType)
						{
							item_type_str = "SPR";
							id = item.spr->GetSpriteID();
						}

						else
						{
							// BAD TYPE
						}

						id_size = to_string(id.size());

						ExtendString(layer, 8, "!");
						ExtendString(x, 12, "@");
						ExtendString(y, 12, "#");
						ExtendString(id_size, 8, "$");

						scene_file.write(item_type_str.c_str(), item_type_str.size());
						scene_file.write(layer.c_str(), 8);
						scene_file.write(x.c_str(), 12);
						scene_file.write(y.c_str(), 12);
						scene_file.write(id_size.c_str(), 8);
						scene_file.write(id.c_str(), id.size());

						// Generate a garbage string of 8 characters and write it to the file stream
						// To further limit readability of the outputted scene file
						for (int i = 0; i < 8; i++)
						{
							scene_garbage_str[i] = (char)rand() % 200;
						}

						scene_garbage_str[9] = '\0';
						scene_file.write(scene_garbage_str, 8);
					}
				}

				UnsavedChanges = false;
			}

			else
			{
				ShouldShowFileErrWindow = true;
			}

			scene_file.close();
		}

		void DrawGrid()
		{
			const int line_pixel_size = 5;

			glUseProgram(0);

			GLfloat vertex_buffer[4]
			{
				-0.2f, 0.5f, 0.2f, 0.5f
			};

			VertexBuffer VBO(GL_ARRAY_BUFFER);

			VBO.BufferData(4 * sizeof(GLfloat), vertex_buffer, GL_STATIC_DRAW);
			VBO.VertexAttribPointer(0, 2, GL_FLOAT, 0, 2 * sizeof(GLfloat), (GLvoid*)0);

			VBO.Bind();
			glDrawArrays(GL_LINES, 0, 2);
			VBO.Unbind();
		}

		void _ShowModalWindows()
		{
			// Shows all the modal windows that are flagged true.
			// (Windows such as About me, Support me etc..

			if (ShouldShowAboutTheAuthorWindow)
			{
				ImGui::OpenPopup("About the author");

				if (ImGui::BeginPopupModal("About the author", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ShouldBlockInput = true;

					ImGui::Text("Programmed by Samuel Rasquinha\n");
					ImGui::Text("GitHub : Samuel Rasquinha");
					ImGui::Text("Gmail : samuelrasquinha@gmail.com");
					ImGui::Text("Instagram : Samuel Rasquinha");
					ImGui::Text("Programmed using C++ and OpenGL.");
					ImGui::Text("If you like this program, please consider starring this on github.");

					ImGui::SetItemDefaultFocus();

					if (ImGui::Button("OK", ImVec2(120, 0)))
					{
						ShouldBlockInput = false;
						ShouldShowAboutTheAuthorWindow = false;
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
			}

			if (ShouldShowSupportMeWindow)
			{
				ImGui::OpenPopup("Support me");

				if (ImGui::BeginPopupModal("Support me", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ShouldBlockInput = true;

					ImGui::Text("The entire GLGame project was made by Samuel Rasquinha (samuelrasquinha@gmail.com)");
					ImGui::Text("Since I am only 14 years old, I don't have paypal. But you can show your support by starring this project on GitHub.");

					ImGui::SetItemDefaultFocus();

					if (ImGui::Button("OK", ImVec2(120, 0)))
					{
						ShouldBlockInput = false;
						ShouldShowSupportMeWindow = false;
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
			}

			if (ShouldShowWITWindow)
			{
				ImGui::OpenPopup("What is this?");

				if (ImGui::BeginPopupModal("What is this?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ShouldBlockInput = true;

					ImGui::Text("This is a debug tool and scene editor I made to run along with GLGame");
					ImGui::Text("This is to be run during runtime.");

					ImGui::SetItemDefaultFocus();

					if (ImGui::Button("OK", ImVec2(120, 0)))
					{
						ShouldBlockInput = false;
						ShouldShowWITWindow = false;
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}

			}

			if (ShouldShowDependenciesWindow)
			{
				ImGui::OpenPopup("Dependencies");

				if (ImGui::BeginPopupModal("Dependencies", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ShouldBlockInput = true;

					ImGui::Text("GLGame the minimal open-source dependencies. All of the below are open-source and free");
					ImGui::Text("GLGame uses the following dependencies : ");
					ImGui::Text("C++17 (For std::filesystem)");
					ImGui::Text("GLEW (To access OpenGL functions)");
					ImGui::Text("GLFW (Windowing environment)");
					ImGui::Text("ImGui (Bloat free immediate mode GUI interface by Omar Ocurnut)");
					ImGui::Text("stb_image (To read texture files)\n");

					ImGui::SetItemDefaultFocus();

					if (ImGui::Button("OK", ImVec2(120, 0)))
					{
						ShouldBlockInput = false;
						ShouldShowDependenciesWindow = false;
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
			}

			if (ShouldShowSaveAsWindow)
			{
				ImGui::OpenPopup("Save As");

				if (ImGui::BeginPopupModal("Save As", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ShouldBlockInput = true;

					ImGui::InputText("Path", SceneFilePath, SceneFilePthSize);

					ImGui::SetItemDefaultFocus();

					if (ImGui::Button("OK", ImVec2(120, 0)))
					{
						ShouldBlockInput = false;
						ShouldShowSaveAsWindow = false;
						SceneFilePathSet = true;

						FlushSceneFile();
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
			}

			if (ShouldShowFileErrWindow)
			{
				ImGui::OpenPopup("File error");

				if (ImGui::BeginPopupModal("File error", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ShouldBlockInput = true;

					ImGui::SetItemDefaultFocus();

					ImGui::Text("Error saving file..");
					ImGui::Text("Click OK to continue..");

					if (ImGui::Button("OK", ImVec2(120, 0)))
					{
						ShouldBlockInput = false;
						ShouldShowFileErrWindow = false;
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
			}
		}

		void _RenderSceneEditorItems()
		{
			stbi_set_flip_vertically_on_load(true);

			// Draw all the Scene editor objects that are written in the map
			SceneEditorBatcher->StartSpriteBatch(SceneEditorCamera, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

			for (auto e = SceneEditorItemQueue.begin(); e != SceneEditorItemQueue.end(); e++)
			{
				for (int i = 0; i < e->second.size(); i++)
				{
					if (e->second.at(i).item_type == SE_ObjectType && e->second.at(i).obj != nullptr)
					{
						SceneEditorBatcher->AddGenericTextureToBatch(e->second.at(i).obj->GetSprite()->GetCurrentTexture(), glm::vec3(e->second.at(i).x, e->second.at(i).y, 1.0f));
					}

					else if (e->second.at(i).item_type == SE_SpriteType && e->second.at(i).spr != nullptr)
					{
						SceneEditorBatcher->AddGenericTextureToBatch(e->second.at(i).spr->GetCurrentTexture(), glm::vec3(e->second.at(i).x, e->second.at(i).y, 1.0f));
					}
				}
			}

			SceneEditorBatcher->EndSpriteBatch();

			// Object Drawing ends here..

			// Draw the ghost mouse image

			if (CurrentOperationSelected == PlaceItems)
			{
				if (GhostObjectImage.item_type == SE_ObjectType && GhostObjectImage.obj != nullptr
					&& ItemTypeSelected == ObjectSelection
					&& ObjectIDList != nullptr && RadioObjectSelected != -1)
				{
					string item_id = (ObjectIDList->at(RadioObjectSelected));
					unordered_map<string, Object*>::iterator chk = SceneEditorGlobalObjects->find(item_id);

					if (chk != SceneEditorGlobalObjects->end() && SceneEditorGlobalObjects->at(item_id)->HasSprite())
					{
						SceneEditorBatcher->StartSpriteBatch(SceneEditorCamera, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
						SceneEditorBatcher->AddGenericTextureToBatch(GhostObjectImage.obj->GetSprite()->GetCurrentTexture(), glm::vec3(GhostObjectImage.x, GhostObjectImage.y, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));
						SceneEditorBatcher->EndSpriteBatch();
					}
				}

				else if (GhostObjectImage.item_type == SE_SpriteType && GhostObjectImage.spr != nullptr
					&& ItemTypeSelected == SpriteSelection
					&& SpriteIDList != nullptr && RadioSpriteSelected != -1)
				{
					string item_id = (SpriteIDList->at(RadioSpriteSelected));
					unordered_map<string, Sprite*>::iterator chk = SceneEditorGlobalSprites->find(item_id);

					if (chk != SceneEditorGlobalSprites->end())
					{
						SceneEditorBatcher->StartSpriteBatch(SceneEditorCamera, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
						SceneEditorBatcher->AddGenericTextureToBatch(GhostObjectImage.spr->GetCurrentTexture(), glm::vec3(GhostObjectImage.x, GhostObjectImage.y, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));
						SceneEditorBatcher->EndSpriteBatch();
					}
				}
			}

			// Drawing the ghost mouse image ends..
		}

		void _DrawDebugWindow()
		{
			if (CurrentOperationSelected == DebugGame)
			{
				if (ImGui::Begin("Debug.."))
				{
					ImGui::Text("Current Frame : %ld", DebugInfo->CurrentFrame);
					ImGui::Text("Current Time Step : %lf", DebugInfo->CurrentTS);
					ImGui::Text("Render Time : %lf\n", DebugInfo->RenderTime);
					ImGui::Text("Vertex Count : %d", DebugInfo->VerticesCount);
					ImGui::Text("Indices Count : %d", DebugInfo->IndicesCount);
					ImGui::Text("Quad Count : %d", DebugInfo->QuadCount);
					ImGui::Text("Objects Drawn : %d", DebugInfo->ObjectsDrawn);
					ImGui::Text("Sprites Drawn : %d", DebugInfo->SpritesDrawn);
					ImGui::Text("Lights Drawn : %d", DebugInfo->LightsDrawn);

					ImGui::End();
				}
			}
		}

		void _DrawSEWidgets()
		{
			static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
			ImGui::SetNextWindowPos(ImVec2(SceneEditorWidth * 0.75, 20), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);

			if (ImGui::Begin("Operation select."))
			{
				ImGui::Text("What would you like to do ?");
				ImGui::RadioButton("Place GLGame::Items", &CurrentOperationSelected, PlaceItems);
				ImGui::RadioButton("Change/Add Backgrounds", &CurrentOperationSelected, ChangeBackground);
				ImGui::RadioButton("See properties of a GLGame::Object or GLGame::Sprite", &CurrentOperationSelected, SeeProperties);
				ImGui::RadioButton("Debug", &CurrentOperationSelected, DebugGame);
				ImGui::RadioButton("View or Look at the scene.", &CurrentOperationSelected, ViewScene);
				ImGui::Text("\n\nEditor settings : \n");
				ImGui::InputInt("Grid Size X (in pixels)", &GridX);
				ImGui::InputInt("Grid Size Y (in pixels)", &GridY);

				if (GridX < 0)
				{
					GridX = 0;
				}

				if (GridY < 0)
				{
					GridY = 0;
				}

				ImGui::Text("\n");
				ImGui::End();
			}
		}

		void _DrawSEPlaceItemWidgets()
		{
			static bool show_window = true;

			if (CurrentOperationSelected == PlaceItems)
			{
				// Reset the scene editor :
				SceneEditorCamera->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
				SceneEditorCamera->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

				if (ImGui::Begin("Place GLGame::SceneItems", &show_window))
				{
					if (ImGui::CollapsingHeader("Place Scene Items"))
					{
						if (ImGui::TreeNode("Objects"))
						{
							string obj_name_holder;

							for (int i = 0; i < ObjectIDList->size(); i++)
							{
								obj_name_holder = ObjectIDList->at(i);
								obj_name_holder.erase(obj_name_holder.begin(), obj_name_holder.begin() + 5);
								ImGui::RadioButton(obj_name_holder.c_str(), &RadioObjectSelected, i);
							}

							ImGui::TreePop();
						}

						if (ImGui::TreeNode("Sprites"))
						{
							string spr_name_holder;

							for (int i = 0; i < SpriteIDList->size(); i++)
							{
								spr_name_holder = SpriteIDList->at(i);
								spr_name_holder.erase(spr_name_holder.begin(), spr_name_holder.begin() + 5);
								ImGui::RadioButton(spr_name_holder.c_str(), &RadioSpriteSelected, i);
							}

							ImGui::TreePop();
						}

						ImGui::Text("\nTYPE OF SCENE ITEM THAT YOU WOULD LIKE TO PLACE : \n\n");
						ImGui::RadioButton("GLGame::Object", &ItemTypeSelected, ObjectSelection);
						ImGui::RadioButton("GLGame::Sprite", &ItemTypeSelected, SpriteSelection);
						// Draw the "Selected Item" ImGui window
					}

					ImGui::Text("\n\n");
					ImGui::InputFloat("Layer/Depth", &CurrentSceneEditorLayer);
				}

				ImGui::End();

				// Draw Selected item window
				{
					ImGuiWindowFlags selected_item_window_flags = 0;
					bool selected_item_show_window = true;

					//selected_item_window_flags |= ImGuiWindowFlags_NoTitleBar;
					//selected_item_window_flags |= ImGuiWindowFlags_NoMove;
					//selected_item_window_flags |= ImGuiWindowFlags_NoNav;
					//selected_item_window_flags |= ImGuiWindowFlags_NoBackground;

					selected_item_window_flags |= ImGuiWindowFlags_NoScrollbar;
					selected_item_window_flags |= ImGuiWindowFlags_MenuBar;
					selected_item_window_flags |= ImGuiWindowFlags_NoResize;
					selected_item_window_flags |= ImGuiWindowFlags_NoCollapse;
					selected_item_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
					selected_item_window_flags |= ImGuiWindowFlags_NoDocking;

					Sprite* selected_obj_spr = SceneEditorGlobalObjects->at("@#$*#Object_1")->GetSprite();

					ImGui::SetNextWindowPos(ImVec2(SceneEditorWidth - 270, SceneEditorHeight - 270), ImGuiCond_FirstUseEver);
					ImGui::SetNextWindowSize(ImVec2(250, 250), ImGuiCond_Always);
					ImGui::Begin("Selected Item", &selected_item_show_window, selected_item_window_flags);
					ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "SELECTED OBJECT ! \n\n\n");

					// Add the item to the imgui draw list
					ImGui::GetWindowDrawList()->AddImage(
						(void*)selected_obj_spr->GetCurrentTexture()->GetTextureID(),
						ImVec2(ImGui::GetCursorScreenPos()),
						ImVec2(ImGui::GetCursorScreenPos().x + selected_obj_spr->GetCurrentTextureWidth(),
							ImGui::GetCursorScreenPos().y + selected_obj_spr->GetCurrentTextureHeight()),
						ImVec2(1, 0),
						ImVec2(0, 1));

					ImGui::End();
				}

			}
		}

		void _DrawSEMenuBar()
		{
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Save As", ""))
					{
						ShouldShowSaveAsWindow = true;
					}

					if (ImGui::MenuItem("Save", ""))
					{
						if (SceneFilePathSet == false)
						{
							ShouldShowSaveAsWindow = true;
						}

						else
						{
							fstream scene_file;

							scene_file.open(string(SceneFilePath), ios::out | ios::binary);

							if (scene_file.good() && scene_file.is_open())
							{
								scene_file.close();
								FlushSceneFile();
							}

							else
							{
								ShouldShowFileErrWindow = true;
							}
						}
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Edit"))
				{
					if (ImGui::MenuItem("Undo", "CTRL+Z"))
					{
						// Undo
					}

					if (ImGui::MenuItem("Redo", "CTRL+Y", false, false))
					{

					}

					ImGui::Separator();

					if (ImGui::MenuItem("Cut", "CTRL+X"))
					{

					}

					if (ImGui::MenuItem("Copy", "CTRL+C"))
					{

					}

					if (ImGui::MenuItem("Paste", "CTRL+V"))
					{

					}


					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Help"))
				{
					if (ImGui::MenuItem("About the author"))
					{
						ShouldShowAboutTheAuthorWindow = true;
					}

					else
					{
						ShouldShowAboutTheAuthorWindow = false;
					}

					if (ImGui::MenuItem("What is this?"))
					{
						ShouldShowWITWindow = true;
					}

					else
					{
						ShouldShowWITWindow = false;
					}


					if (ImGui::MenuItem("Dependencies"))
					{
						ShouldShowDependenciesWindow = true;
					}

					else
					{
						ShouldShowDependenciesWindow = false;
					}

					if (ImGui::MenuItem("Support"))
					{
						ShouldShowSupportMeWindow = true;
					}

					else
					{
						ShouldShowSupportMeWindow = false;
					}

					ImGui::EndMenu();
				}

				ImGui::EndMainMenuBar();
			}
		}

		void _SetSceneEditorCloseFlag(bool val)
		{
			SE_window_destroyed = true;
		}

		bool RenderSceneEditor()
		{
			if (SE_window_destroyed == false)
			{
				glfwMakeContextCurrent(SceneEditorWindow);
				glfwGetFramebufferSize(SceneEditorWindow, &SceneEditorWidth, &SceneEditorWidth);
				glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);

				// Draw the ImGui items
				ImGui::SetCurrentContext(imcontext);

				if (ImGui::IsAnyWindowHovered())
				{
					ShouldBlockInput = true;
				}

				else
				{
					ShouldBlockInput = false;
				}

				// Render the test item
				_RenderSceneEditorItems();

				bool display_title_place_item = true;
				bool display_title_selected_item = true;

				ImGui::SetNextWindowPos(ImVec2(SceneEditorWidth * 0.75, 20), ImGuiCond_FirstUseEver);
				ImGui::SetNextWindowSize(ImVec2(555, 760), ImGuiCond_FirstUseEver);

				ImGui::SetCurrentContext(imcontext);
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				_DrawSEMenuBar();
				_ShowModalWindows();
				_DrawSEWidgets();
				_DrawSEPlaceItemWidgets();
					
				// Draws the debug window if that operation is actually selected
				_DrawDebugWindow();

				// Scope : show modal window when you click on the close button
				{
					if (ShouldShowCloseModalWindow)
					{
						if (UnsavedChanges)
						{
							ImGui::SetNextWindowSize(ImVec2(600, 100), ImGuiCond_Always);
							ImGui::OpenPopup("Delete?");

							if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
							{
								ImGui::Text("The Scene you just created will be lost forever! This action cannot be undone!\n");
								ImGui::Separator();
								ShouldBlockInput = true;

								if (ImGui::Button("OK", ImVec2(120, 0)))
								{
									ShouldBlockInput = false;
									ImGui::CloseCurrentPopup();

									if (SE_window_destroyed == false)
									{
										SE_window_destroyed = true;
										glfwDestroyWindow(SceneEditorWindow);
										Log::LogToConsole("The Scene Editor was destroyed!");
									}

									return 0;
								}

								ImGui::SetItemDefaultFocus();
								ImGui::SameLine();

								if (ImGui::Button("Cancel", ImVec2(120, 0)))
								{
									ShouldBlockInput = false;
									ShouldShowCloseModalWindow = false;
									ImGui::CloseCurrentPopup();
								}

								ImGui::EndPopup();
							}
						}

						else
						{
							ShouldBlockInput = false;

							if (SE_window_destroyed == false)
							{
								SE_window_destroyed = true;
								glfwDestroyWindow(SceneEditorWindow);
								Log::LogToConsole("The Scene Editor was destroyed!");
							}

							return 0;
						}
					}
				}


				ImGui::Render();
				ImGui::EndFrame();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				glfwSwapBuffers(SceneEditorWindow);
				return true;
			}

			else
			{
				return false;
			}

			return false;
		}

		bool SceneEditorAlive()
		{
			return SE_window_destroyed;
		}

		void SEWindowResizeCallback(GLFWwindow* window, int width, int height)
		{
			if (window != SceneEditorWindow)
			{
				return;
			}

			glfwGetFramebufferSize(window, &WindowSizeX, &WindowSizeY);
			glfwGetCursorPos(window, &MousePosX, &MousePosY);

			// Set the view port
			glViewport(0, 0, WindowSizeX, WindowSizeY);
		}

		// function to snap an object to a particular grid
		float SnapToGrid(int value, int size)
		{
			if (size == 0)
			{
				size = 1;
			}

			int temp = value % size;

			if (temp < (size / 2))
			{
				return (value - temp);
			}

			else
			{
				return (value + size) - temp;
			}
		}

		void SEKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (window != SceneEditorWindow)
			{
				return;
			}

			if (ShouldBlockInput)
			{
				return;
			}

			glfwGetCursorPos(window, &MousePosX, &MousePosY);

			if (action == GLFW_PRESS)
			{
				if (key == GLFW_KEY_S && mods & GLFW_MOD_CONTROL)
				{
					if (SceneFilePathSet == false)
					{
						ShouldShowSaveAsWindow = true;

					}

					else
					{
						fstream scene_file;

						scene_file.open(string(SceneFilePath), ios::out | ios::binary);

						if (scene_file.good() && scene_file.is_open())
						{
							scene_file.close();
							FlushSceneFile();
						}

						else
						{
							ShouldShowFileErrWindow = true;
						}
					}
				}
			}

			else if (action == GLFW_RELEASE)
			{

			}

			else
			{

			}
		}

		void SECursorPosCallback(GLFWwindow* window, double xpos, double ypos)
		{
			if (window != SceneEditorWindow)
			{
				return;
			}

			if (ShouldBlockInput)
			{
				return;
			}

			if (CurrentOperationSelected == PlaceItems)
			{
				// Draw a ghost image of the current selected item

				if (ItemTypeSelected == ObjectSelection)
				{
					if (ObjectIDList != nullptr && RadioObjectSelected != -1 && RadioObjectSelected < ObjectIDList->size() && RadioObjectSelected >= 0)
					{
						int width, height;
						string item_id;

						item_id = (ObjectIDList->at(RadioObjectSelected));
						unordered_map<string, Object*>::iterator chk = SceneEditorGlobalObjects->find(item_id);

						// If the object exists in the map
						if (chk != SceneEditorGlobalObjects->end())
						{
							if (SceneEditorGlobalObjects->at(item_id)->HasSprite())
							{
								GhostObjectImage.item_type = SE_ObjectType;
								GhostObjectImage.obj = SceneEditorGlobalObjects->at(item_id);

								// Get the width and height of the textures
								width = GhostObjectImage.obj->GetSprite()->GetCurrentTextureWidth();
								height = GhostObjectImage.obj->GetSprite()->GetCurrentTextureWidth();

								// Subtract width/2 and height/2 from MouseX and MouseY so that the origin of the object is in the center

								GhostObjectImage.x = (float)xpos - ((int)width / 2);
								GhostObjectImage.y = (float)(SceneEditorHeight - ypos);
								GhostObjectImage.y -= ((int)height / 2);

								// Snap the Ghost object to the grid
								GhostObjectImage.x = SnapToGrid(GhostObjectImage.x, GridX);
								GhostObjectImage.y = SnapToGrid(GhostObjectImage.y, GridY);
							}
						}
					}
				}

				else if (ItemTypeSelected == SpriteSelection)
				{
					if (SpriteIDList != nullptr && RadioSpriteSelected != -1 && RadioSpriteSelected < SpriteIDList->size() && RadioSpriteSelected >= 0)
					{
						int width, height;
						string item_id;

						item_id = (SpriteIDList->at(RadioSpriteSelected));
						unordered_map<string, Sprite*>::iterator chk = SceneEditorGlobalSprites->find(item_id);

						// If the object exists in the map
						if (chk != SceneEditorGlobalSprites->end())
						{
							GhostObjectImage.item_type = SE_SpriteType;
							GhostObjectImage.spr = SceneEditorGlobalSprites->at(item_id);

							width = GhostObjectImage.spr->GetCurrentTextureWidth();
							height = GhostObjectImage.spr->GetCurrentTextureWidth();

							// Subtract width/2 and height/2 from MouseX and MouseY so that the origin of the object is in the center

							GhostObjectImage.x = (float)xpos - ((int)width / 2);
							GhostObjectImage.y = (float)(SceneEditorHeight - ypos);
							GhostObjectImage.y -= ((int)height / 2);

							// Snap the Ghost object to the grid
							GhostObjectImage.x = SnapToGrid(GhostObjectImage.x, GridX);
							GhostObjectImage.y = SnapToGrid(GhostObjectImage.y, GridY);
						}
					}
				}
			}

			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
			{
				return;
			}

			// Sneaky way to handle mouse drag events
			if (CurrentOperationSelected == ViewScene)
			{
				glfwGetCursorPos(window, &MousePosX, &MousePosY);
				SceneEditorCamera->SetPosition(glm::vec3(StartPanX - MousePosX, MousePosY - StartPanY, 1.0f));
			}
		}

		void SEMouseCallback(GLFWwindow* window, int button, int action, int mods)
		{
			if (window != SceneEditorWindow)
			{
				return;
			}

			if (ShouldBlockInput)
			{
				return;
			}

			glfwGetCursorPos(window, &MousePosX, &MousePosY);

			if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)
			{
				// Set the unsaved changes flag
				UnsavedChanges = true;

				StartPanX = MousePosX;
				StartPanY = MousePosY;

				if (CurrentOperationSelected == PlaceItems)
				{
					if (ItemTypeSelected == ObjectSelection &&
						RadioObjectSelected < ObjectIDList->size() && RadioObjectSelected > -1)
					{
						if (ObjectIDList != nullptr)
						{
							SceneEditorRenderItem item;
							int width, height;
							string item_id;

							item_id = (ObjectIDList->at(RadioObjectSelected));

							unordered_map<string, Object*>::iterator chk = SceneEditorGlobalObjects->find(item_id);

							// If the object exists in the map
							if (chk != SceneEditorGlobalObjects->end())
							{
								if (SceneEditorGlobalObjects->at(item_id)->HasSprite())
								{
									item.item_type = SE_ObjectType;
									item.obj = SceneEditorGlobalObjects->at(item_id);

									// Get the width and height of the textures
									width = item.obj->GetSprite()->GetCurrentTextureWidth();
									height = item.obj->GetSprite()->GetCurrentTextureHeight();

									// Subtract width/2 and height/2 from MouseX and MouseY so that the origin of the object is in the center

									item.x = (float)MousePosX - ((int)width / 2);
									item.y = (float)(SceneEditorHeight - MousePosY);
									item.y -= ((int)height / 2);

									// Snap the X and Y to the grid
									item.x = SnapToGrid(item.x, GridX);
									item.y = SnapToGrid(item.y, GridY);

									item.layer = CurrentSceneEditorLayer;
									SceneEditorItemQueue[item.layer].push_back(item);
								}
							}
						}
					}

					else if (ItemTypeSelected == SpriteSelection &&
						RadioSpriteSelected < SpriteIDList->size() && RadioSpriteSelected > -1)
					{
						if (SpriteIDList != nullptr)
						{
							SceneEditorRenderItem item;
							int width, height;
							string item_id;

							item_id = (SpriteIDList->at(RadioSpriteSelected));

							unordered_map<string, Sprite*>::iterator chk = SceneEditorGlobalSprites->find(item_id);

							// If the object exists in the map
							if (chk != SceneEditorGlobalSprites->end())
							{
								if (SceneEditorGlobalSprites->at(item_id))
								{
									item.item_type = SE_SpriteType;
									item.spr = SceneEditorGlobalSprites->at(item_id);

									// Get the width and height of the textures
									width = item.spr->GetCurrentTextureWidth();
									height = item.spr->GetCurrentTextureHeight();

									// Subtract width/2 and height/2 from MouseX and MouseY so that the origin of the object is in the center

									item.x = (float)MousePosX - ((int)width / 2);
									item.y = (float)(SceneEditorHeight - MousePosY);
									item.y -= ((int)height / 2);

									// Snap the X and Y to the grid
									item.x = SnapToGrid(item.x, GridX);
									item.y = SnapToGrid(item.y, GridY);

									item.layer = CurrentSceneEditorLayer;
									SceneEditorItemQueue[item.layer].push_back(item);
								}
							}
						}
					}
				}
			}

			if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_RIGHT)
			{
				// Set the unsaved changes flag
				UnsavedChanges = true;

				AABB o1, o2;

				o1.x = (float)MousePosX;
				o1.y = (float)(SceneEditorHeight - MousePosY);
				o1.w = 4;
				o1.h = 4;

				for (int i = 0; i < SceneEditorItemQueue[CurrentSceneEditorLayer].size(); i++)
				{
					o2.x = SceneEditorItemQueue[CurrentSceneEditorLayer].at(i).x;
					o2.y = SceneEditorItemQueue[CurrentSceneEditorLayer].at(i).y;

					if (SceneEditorItemQueue[CurrentSceneEditorLayer].at(i).item_type == SE_ObjectType)
					{
						o2.w = SceneEditorItemQueue[CurrentSceneEditorLayer].at(i).obj->GetSprite()->GetCurrentTextureWidth();
						o2.h = SceneEditorItemQueue[CurrentSceneEditorLayer].at(i).obj->GetSprite()->GetCurrentTextureHeight();

						// If the mouse collided with the object, erase it from the editor queue
						if (CheckAABBCollision(o1, o2))
						{
							SceneEditorItemQueue[CurrentSceneEditorLayer].erase(SceneEditorItemQueue[CurrentSceneEditorLayer].begin() + i);
							break;
						}
					}

					else if (SceneEditorItemQueue[CurrentSceneEditorLayer].at(i).item_type == SE_SpriteType)
					{
						o2.w = SceneEditorItemQueue[CurrentSceneEditorLayer].at(i).spr->GetCurrentTextureWidth();
						o2.h = SceneEditorItemQueue[CurrentSceneEditorLayer].at(i).spr->GetCurrentTextureHeight();

						if (CheckAABBCollision(o1, o2))
						{
							SceneEditorItemQueue[CurrentSceneEditorLayer].erase(SceneEditorItemQueue[CurrentSceneEditorLayer].begin() + i);
							break;
						}
					}
				}
			}

			else if (action == GLFW_RELEASE)
			{

			}

			else
			{

			}
		}

		void SEScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
		{
			if (window != SceneEditorWindow)
			{
				return;
			}

			if (ShouldBlockInput)
			{
				return;
			}

			glfwGetCursorPos(window, &MousePosX, &MousePosY);

			if (CurrentOperationSelected == ViewScene)
			{
				if (yoffset < 0)
				{
					const glm::vec3 scale = SceneEditorCamera->GetScale();
					SceneEditorCamera->SetScale(glm::vec3(scale.x - 0.1, scale.y - 0.1, 1.0f));
				}

				else if (yoffset > 0)
				{
					const glm::vec3 scale = SceneEditorCamera->GetScale();
					SceneEditorCamera->SetScale(glm::vec3(scale.x + 0.1, scale.y + 0.1, 1.0f));
				}
			}
		}

		void SEWindowCloseCallback(GLFWwindow* window)
		{
			if (window != SceneEditorWindow)
			{
				return;
			}

			ShouldShowCloseModalWindow = true;
		}
	}
}

// Scene Editor End..