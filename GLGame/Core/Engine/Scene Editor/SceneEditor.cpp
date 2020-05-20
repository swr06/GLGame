#include "SceneEditor.h"

// The GLGame Scene editor has to be called at runtime. 

namespace GLGame
{
	namespace SceneEditor
	{
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
			Debug,
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

		// Render queue
		static map<int, vector<SceneEditorRenderItem>> SceneEditorItemQueue;

		static Shader SceneEditorRenderItemShader;

		// Mouse and window attributes
		static double MousePosX = 0;
		static double MousePosY = 0;
		static int WindowSizeX = 0;
		static int WindowSizeY = 0;
		static double MouseScrollOffsetX = 0;
		static double MouseScrollOffsetY = 0;

		static const char* ObjectInsertString = "@#$*#\0";
		static const char* SpriteInsertString = "$*$^(\0";

		// Modal windows
		static bool ShouldShowCloseModalWindow = false;

		static bool ShouldShowAboutTheAuthorWindow = false;
		static bool ShouldShowSupportMeWindow = false;
		static bool ShouldShowWITWindow = false; // What is this window
		static bool ShouldShowDependenciesWindow = false;

		// IMGUI context => Scene editor window
		ImGuiContext* imcontext;

		// Scene editor matrices
		Camera* SceneEditorCamera;

		// Other needed structures
		int CurrentOperationSelected = 0; // Has to be an int for imgui. Used as an enum class "Operations" 

		GLFWwindow* InitSceneEditor(unordered_map<string, Object*>* global_objects, unordered_map<string, Sprite*>* global_sprites, vector<string>* objid_list, vector<string>* sprid_list, GLFWwindow* window, ImGuiContext* context)
		{
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

			SceneEditorCamera = new Camera(0.0f, (float)SceneEditorWidth, 0.0f, (float)SceneEditorHeight);
			SceneEditorBatcher = new SpriteBatcher();
			SceneEditorRenderItemShader.CreateShaderProgram(GLGAME_DEFAULT_SE_VERTEX, GLGAME_DEFAULT_SE_FRAGMENT);
			return SceneEditorWindow;
		}

		void _SetSEImGuiFlags() 
		{

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
					ImGui::Text("Programmed by Samuel Rasquinha\n");
					ImGui::Text("GitHub : Samuel Rasquinha");
					ImGui::Text("Gmail : samuelrasquinha@gmail.com");
					ImGui::Text("Instagram : Samuel Rasquinha");
					ImGui::Text("Programmed using C++ and OpenGL.");
					ImGui::Text("If you like this program, please consider starring this on github.");

					ImGui::SetItemDefaultFocus();

					if (ImGui::Button("OK", ImVec2(120, 0)))
					{
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
					ImGui::Text("The entire GLGame project was made by Samuel Rasquinha (samuelrasquinha@gmail.com)");
					ImGui::Text("Since I am only 14 years old, I don't have paypal. But you can show your support by starring this project on GitHub.");

					ImGui::SetItemDefaultFocus();

					if (ImGui::Button("OK", ImVec2(120, 0)))
					{
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
					ImGui::Text("This is a debug tool and scene editor I made to run along with GLGame");
					ImGui::Text("This is to be run during runtime.");

					ImGui::SetItemDefaultFocus();

					if (ImGui::Button("OK", ImVec2(120, 0)))
					{
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
						ShouldShowDependenciesWindow = false;
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
			}
		}

		void RenderSceneEditorItems()
		{
			stbi_set_flip_vertically_on_load(true);

			SceneEditorBatcher->StartSpriteBatch(SceneEditorCamera);

			for (auto e = SceneEditorItemQueue.begin(); e != SceneEditorItemQueue.end(); e++)
			{
				for (int i = 0; i < e->second.size(); i++)
				{
					SceneEditorBatcher->AddGenericTextureToBatch(e->second.at(i).tex, glm::vec3(e->second.at(i).x, e->second.at(i).y, 1.0f));
				}
			}

			SceneEditorBatcher->EndSpriteBatch();

		}

		void _DrawSEWidgets()
		{
			ImGui::SetNextWindowPos(ImVec2(SceneEditorWidth * 0.75, 20), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);

			ImGui::Begin("Operation select.");
			ImGui::Text("What would you like to do ?");
			ImGui::RadioButton("Place GLGame::Items", &CurrentOperationSelected, PlaceItems);
			ImGui::RadioButton("Change/Add Backgrounds", &CurrentOperationSelected, ChangeBackground);
			ImGui::RadioButton("See properties of a GLGame::Object or GLGame::Sprite", &CurrentOperationSelected, SeeProperties);
			ImGui::RadioButton("Debug", &CurrentOperationSelected, Debug);
			ImGui::RadioButton("View or Look at the scene.", &CurrentOperationSelected, ViewScene);
			ImGui::End();
		}

		void _DrawSEPlaceItemWidgets()
		{
			static bool show_window = true;

			if (CurrentOperationSelected == PlaceItems)
			{
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
				}

				ImGui::End();

				{
					ImGuiWindowFlags selected_item_window_flags = 0;
					//selected_item_window_flags |= ImGuiWindowFlags_NoTitleBar;
					//selected_item_window_flags |= ImGuiWindowFlags_NoMove;
					//selected_item_window_flags |= ImGuiWindowFlags_NoNav;
					bool selected_item_show_window = true;
					selected_item_window_flags |= ImGuiWindowFlags_NoScrollbar;
					selected_item_window_flags |= ImGuiWindowFlags_MenuBar;
					selected_item_window_flags |= ImGuiWindowFlags_NoResize;
					selected_item_window_flags |= ImGuiWindowFlags_NoCollapse;
					selected_item_window_flags |= ImGuiWindowFlags_NoBackground;
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
						ImVec2(0, 1),
						ImVec2(1, 0));

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
						// Open Save AS
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

				// Render the test item
				RenderSceneEditorItems();

				bool display_title_place_item = true;
				bool display_title_selected_item = true;

				ImGui::SetNextWindowPos(ImVec2(SceneEditorWidth * 0.75, 20), ImGuiCond_Always);
				ImGui::SetNextWindowSize(ImVec2(555, 760), ImGuiCond_Always);

				ImGui::SetCurrentContext(imcontext);
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				_DrawSEMenuBar();
				_ShowModalWindows();
				_DrawSEWidgets();
				_DrawSEPlaceItemWidgets();

				// Scope : show modal window when you click on the close button
				{
					if (ShouldShowCloseModalWindow)
					{
						ImGui::SetNextWindowSize(ImVec2(600, 100), ImGuiCond_Always);
						ImGui::OpenPopup("Delete?");

						if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
						{
							ImGui::Text("The Scene you just created will be lost forever! This action cannot be undone!\n");
							ImGui::Separator();

							if (ImGui::Button("OK", ImVec2(120, 0)))
							{ 
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
								ShouldShowCloseModalWindow = false;
								ImGui::CloseCurrentPopup(); 
							}

							ImGui::EndPopup();
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

		void CheckSelectedObject()
		{

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

		void SEKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (window != SceneEditorWindow)
			{
				return;
			}

			glfwGetCursorPos(window, &MousePosX, &MousePosY);

			if (action == GLFW_PRESS)
			{
				
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

			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
			{
				return;
			}

			glfwGetCursorPos(window, &MousePosX, &MousePosY);
			SceneEditorCamera->SetPosition(glm::vec3(StartPanX - MousePosX, MousePosY - StartPanY, 1.0f));
		}

		void SEMouseCallback(GLFWwindow* window, int button, int action, int mods)
		{
			if (window != SceneEditorWindow)
			{
				return;
			}

			glfwGetCursorPos(window, &MousePosX, &MousePosY);
			std::cout << MousePosX << "    " << MousePosY << std::endl;

			if (action == GLFW_PRESS)
			{
				StartPanX = MousePosX;
				StartPanY = MousePosY;

				if (CurrentOperationSelected == PlaceItems && ItemTypeSelected == ObjectSelection &&
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
								item.tex = SceneEditorGlobalObjects->at(item_id)->GetSprite()->GetCurrentTexture();
								
								width = item.tex->GetWidth();
								height = item.tex->GetHeight();

								// Subtract width/2 and height/2 from MouseX and MouseY so that the origin of the object is in the center

								item.x = (float)MousePosX - ((int)width / 2);
								item.y = (float)(SceneEditorHeight - MousePosY);
								item.y -= ((int)height / 2);
								item.layer = 0;
								SceneEditorItemQueue[item.layer].push_back(item);
							}
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

			glfwGetCursorPos(window, &MousePosX, &MousePosY);

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