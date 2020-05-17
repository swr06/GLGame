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

		// Variables to initialize the window
		static bool SceneEditorInitialized = false;
		static GLFWwindow* SceneEditorWindow;
		static int SceneEditorWidth = 1366;
		static int SceneEditorHeight = 750;
		static bool SE_window_destroyed = false;

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
		static map<int, vector<SceneEditorRenderItem>> SceneEditorRenderQueue;
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

		// IMGUI context => Scene editor window
		ImGuiContext* imcontext;

		// Scene editor matrices
		Camera* SceneEditorCamera;

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

		void RenderSceneEditorItems()
		{
			stbi_set_flip_vertically_on_load(true);

			Object* obj = SceneEditorGlobalObjects->at("@#$*#Object_1");

			SceneEditorBatcher->StartSpriteBatch(SceneEditorCamera);
			SceneEditorBatcher->AddGenericTextureToBatch(obj->GetSprite()->GetCurrentTexture(), glm::vec3(100.0f, 100.0f, 1.0f));
			SceneEditorBatcher->EndSpriteBatch();

		}

		void _DrawSEWidgets()
		{
			if (ImGui::CollapsingHeader("Scene Items") == false)
			{
				// error
				return;
			}

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

				ImGui::EndMainMenuBar();
			}
		}

		bool RenderSceneEditor()
		{
			glfwGetFramebufferSize(SceneEditorWindow, &SceneEditorWidth, &SceneEditorWidth);

			if (!glfwWindowShouldClose(SceneEditorWindow) && SE_window_destroyed == false)
			{
				glfwMakeContextCurrent(SceneEditorWindow);
				glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);

				// Render the test item
				RenderSceneEditorItems();

				// Draw the ImGui items
				ImGui::SetCurrentContext(imcontext);

				bool display_title_place_item = true;
				bool display_title_selected_item = true;

				ImGuiWindowFlags window_flags_place_item = 0;
				window_flags_place_item |= ImGuiWindowFlags_MenuBar;
				window_flags_place_item |= ImGuiWindowFlags_NoCollapse;
				window_flags_place_item |= ImGuiWindowFlags_NoNav;

				ImGui::SetNextWindowPos(ImVec2(SceneEditorWidth * 0.75, 20), ImGuiCond_Always);
				ImGui::SetNextWindowSize(ImVec2(555, 760), ImGuiCond_Always);

				ImGui::SetCurrentContext(imcontext);
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				ImGui::Begin("GLGame Editor", &display_title_place_item, window_flags_place_item);

				_DrawSEMenuBar();
				_DrawSEWidgets();

				ImGui::End();

				ImGuiWindowFlags window_flags_selected_item = 0;
				window_flags_selected_item |= ImGuiWindowFlags_MenuBar;
				window_flags_selected_item |= ImGuiWindowFlags_NoCollapse;
				window_flags_selected_item |= ImGuiWindowFlags_NoNav;

				{
					Sprite* selected_obj_spr = SceneEditorGlobalObjects->at("@#$*#Object_1")->GetSprite();

					ImGui::Begin("Selected Item", &display_title_selected_item, window_flags_selected_item);

					ImGui::SetNextWindowPos(ImVec2(SceneEditorWidth - 270, SceneEditorHeight - 270), ImGuiCond_FirstUseEver);
					ImGui::SetNextWindowSize(ImVec2(250, 250), ImGuiCond_Always);

					ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "SELECTED OBJECT ! \n\n\n");

					ImGui::GetWindowDrawList()->AddImage(
						(void*)selected_obj_spr->GetCurrentTexture()->GetTextureID(),
						ImVec2(ImGui::GetCursorScreenPos()),
						ImVec2(ImGui::GetCursorScreenPos().x + selected_obj_spr->GetCurrentTextureWidth(),
							ImGui::GetCursorScreenPos().y + selected_obj_spr->GetCurrentTextureHeight()),
						ImVec2(0, 1),
						ImVec2(1, 0));

					ImGui::End();
				}


				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				glfwSwapBuffers(SceneEditorWindow);
				return true;
			}

			else
			{ 
				if (SE_window_destroyed == false)
				{
					SE_window_destroyed = true;
					glfwDestroyWindow(SceneEditorWindow);
					Log::LogToConsole("The Scene Editor was destroyed!");
				}

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

		void SEMouseCallback(GLFWwindow* window, int button, int action, int mods)
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
		}
	}
}