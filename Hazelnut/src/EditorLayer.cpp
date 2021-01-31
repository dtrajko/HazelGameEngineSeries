#include "EditorLayer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Hazel/Core/Input.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Hazel/Scene/SceneSerializer.h"
#include "Hazel/Utils/PlatformUtils.h"

#include "ImGuizmo.h"

#include "Hazel/Math/Math.h"


namespace Hazel {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(16.f / 9.f, false)
	{
		RendererAPI::SetMode(RendererAPI::Mode::Renderer2D);
	}

	void EditorLayer::OnAttach()
	{
		HZ_PROFILE_FUNCTION();

		m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_IDFramebuffer = Framebuffer::Create(fbSpec);

		m_ActiveScene = CreateRef<Scene>();

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

#if 0
		// Entity
		auto square = m_ActiveScene->CreateEntity("Green Square");
		square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

		auto redSquare = m_ActiveScene->CreateEntity("Red Square");
		redSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

		m_SquareEntity = square;

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_SecondCamera = m_ActiveScene->CreateEntity("Camera B");
		auto& cc = m_SecondCamera.AddComponent<CameraComponent>();
		cc.Primary = false;

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{
				std::cout << "CameraController::OnCreate!" << std::endl;

				// two cameras on different locations
				auto& translation = GetComponent<TransformComponent>().Translation;
				translation.x = rand() % 10 - 5.0f;
			}

			void OnDestroy()
			{
				std::cout << "CameraController::OnDestroy!" << std::endl;
			}

			void OnUpdate(Timestep ts)
			{
				// std::cout << "CameraController::Timestep: " << ts << std::endl;

				auto& translation = GetComponent<TransformComponent>().Translation;
				float speed = 5.0f;

				if (Input::IsKeyPressed(KeyCode::A))
					translation.x -= speed * ts;
				if (Input::IsKeyPressed(KeyCode::D))
					translation.x += speed * ts;
				if (Input::IsKeyPressed(KeyCode::W))
					translation.y += speed * ts;
				if (Input::IsKeyPressed(KeyCode::S))
					translation.y -= speed * ts;
			}
		};

		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		// SceneSerializer serializer(m_ActiveScene);
		// serializer.Serialize("assets/scenes/Example.hazel");
		// serializer.Deserialize("assets/scenes/Example.hazel");
	}

	void EditorLayer::OnDetach()
	{
		HZ_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		HZ_PROFILE_FUNCTION();

		// Resize
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		// Update
		if (m_ViewportFocused)
		{
			m_CameraController.OnUpdate(ts);
		}

		m_EditorCamera.OnUpdate(ts);

		// Render
		Renderer2D::ResetStats();
		HZ_PROFILE_SCOPE("Renderer Prep");
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		m_Framebuffer->Bind();

		// Update scene
		m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		auto viewportWidth = m_ViewportBounds[1].x - m_ViewportBounds[0].x;
		auto viewportHeight = m_ViewportBounds[1].y - m_ViewportBounds[0].y;
		my = viewportHeight - my - 26;
		int mouseX = (int)mx;
		int mouseY = (int)my;
		if (mouseX >= 0 && mouseY >= 0 && mouseX < viewportWidth && mouseY < viewportHeight)
		{
			int pixel = m_ActiveScene->Pixel((int)mx, (int)my);
			// HZ_CORE_WARN("[ MX {0} MY {1} ] ID = {2}", mx, my, pixel);

			m_HoveredEntity = pixel == -1 ? Entity() : Entity((entt::entity)pixel, m_ActiveScene.get());
		}

		// m_ActiveScene->DrawIDBuffer(m_IDFramebuffer, m_EditorCamera);
		// m_ActiveScene->OnUpdateRuntime(ts);

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		HZ_PROFILE_FUNCTION();

		// Note: Switch this to true to enable dockspace
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		{
			ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			// DockSpace
			ImGuiIO& io = ImGui::GetIO();
			ImGuiStyle& style = ImGui::GetStyle();
			float minWinSize = style.WindowMinSize.x;
			style.WindowMinSize.x = 336.0f; // TheCherno: 370.0f
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}
			style.WindowMinSize.x = minWinSize;

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					// Disabling fullscreen would allow the window to be moved to the front of other windows, 
					// which we can't undo at the moment without finer window depth/z control.
					//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

					if (ImGui::MenuItem("New", "Ctrl+N")) {
						NewScene();
					}

					if (ImGui::MenuItem("Open...", "Ctrl+O")) {
						OpenScene();
					}

					if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
						SaveSceneAs();
					}

					if (ImGui::MenuItem("Exit")) {
						Application::Get().Close();
					}

					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			m_SceneHierarchyPanel.OnImGuiRender();

			ImGui::Begin("Stats");

			auto stats = Renderer2D::GetStats();
			ImGui::Text("Renderer2D Stats:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

			std::string name = "Null";
			if ((entt::entity)m_HoveredEntity != entt::null && m_HoveredEntity.IsValid())
			{
				name = m_HoveredEntity.GetComponent<TagComponent>().Tag;
			}
			ImGui::Text("Hovered Entity: %s [ID=%d]", name.c_str(), (uint32_t)m_HoveredEntity);

			//	{
			//		m_Framebuffer->Bind();
			//		ImGui::Text("Color Buffer");
			//		ImGui::Image((void*)(intptr_t)m_Framebuffer->GetColorAttachmentRendererID(), { 128.0f, 128.0f });
			//		m_Framebuffer->Unbind();
			//	
			//		m_IDFramebuffer->Bind();
			//		ImGui::Text("ID Buffer");
			//		ImGui::Image((void*)(intptr_t)m_IDFramebuffer->GetIDAttachmentRendererID(), { 128.0f, 128.0f });
			//		m_IDFramebuffer->Unbind();
			//	}

			ImGui::End();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
			ImGui::Begin("Viewport");
			{
				auto viewportOffset = ImGui::GetCursorPos(); // includes tab bar

				m_ViewportFocused = ImGui::IsWindowFocused();
				m_ViewportHovered = ImGui::IsWindowHovered();
				Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

				ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

				if (m_ViewportSize != *((glm::vec2*) & viewportPanelSize) && viewportPanelSize.x > 0 && viewportPanelSize.y > 0)
				{
					m_ViewportSize = glm::vec2(viewportPanelSize.x, viewportPanelSize.y);
				}

				uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
				ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

				auto windowSize = ImGui::GetWindowSize();
				ImVec2 minBound = ImGui::GetWindowPos();
				minBound.x += viewportOffset.x;
				minBound.y += viewportOffset.y;

				ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
				m_ViewportBounds[0] = { minBound.x, minBound.y };
				m_ViewportBounds[1] = { maxBound.x, maxBound.y };

				// Gizmos
				Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
				if (selectedEntity && m_GizmoType != -1)
				{
					ImGuizmo::SetOrthographic(false);
					ImGuizmo::SetDrawlist();

					float windowWidth = (float)ImGui::GetWindowWidth();
					float windowHeight = (float)ImGui::GetWindowHeight();
					ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

					// Camera
					// Runtime camera from entity
					// auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
					// const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
					// const glm::mat4& cameraProjection = camera.GetProjection();
					// glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

					// Editor camera
					const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
					glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

					// Entity transform
					auto& tc = selectedEntity.GetComponent<TransformComponent>();
					glm::mat4 transform = tc.GetTransform();

					// Snapping
					bool snap = Input::IsKeyPressed(Key::LeftControl);
					float snapValue = 0.5f; // Snap to 0.5m for translation/scale
					// Snap to 45 degrees for rotation
					if (m_GizmoType == ImGuizmo::OPERATION::ROTATE) {
						snapValue = 45.0f;
					}

					float snapValues[3] = { snapValue, snapValue, snapValue };

					ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
						(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
						nullptr, snap ? snapValues : nullptr);

					if (ImGuizmo::IsUsing())
					{
						glm::vec3 translation, rotation, scale;
						Math::DecomposeTransform(transform, translation, rotation, scale);

						glm::vec3 deltaRotation = rotation - tc.Rotation;
						tc.Translation = translation;
						tc.Rotation += deltaRotation;
						tc.Scale = scale;
					}
				}
			}
			ImGui::End();
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
		m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(HZ_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		// Shortcuts
		if (e.GetRepeatCount() > 0) {
			return false;
		}

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

		switch (e.GetKeyCode())
		{
			case Key::N:
			{
				if (control)
				{
					NewScene();
				}
				break;
			}
			case Key::O:
			{
				if (control)
				{
					OpenScene();
				}
				break;
			}
			case Key::S:
			{
				if (control && shift)
				{
					SaveSceneAs();
				}
				break;
			}

			// Gizmos
			case Key::Q:
				m_GizmoType = -1;
				break;
			case Key::W:
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case Key::E:
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case Key::R:
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
		}

		return true;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::ButtonLeft && !ImGuizmo::IsUsing() && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
		{
			if ((entt::entity)m_HoveredEntity != entt::null)
			{
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
			}
		}
		return false;
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Hazel Scene (*.hazel)\0*.hazel\0");
		if (!filepath.empty())
		{
			m_ActiveScene = CreateRef<Scene>();
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);

			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(filepath);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Hazel Scene (*.hazel)\0*.hazel\0");
		if (!filepath.empty()) {
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filepath);
		}
	}

}
