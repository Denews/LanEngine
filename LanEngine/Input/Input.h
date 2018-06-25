#pragma once
#include "../Utils/Utils.h"

namespace Lan
{
	class Window;

	using Key = char;

	enum class MouseButton
	{
		Left,
		Right
	};

	class Input final : public Singleton<Input>
	{
	public:
		friend Singleton<Input>;

		void Update();

		//�����ִٸ� true, �׿ܿ� false
		bool IsKeyDown(Key key) const;
		//�ش� Ű�� ���� ù �����ӿ��� true, �׿ܿ� false
		bool IsKeyPressed(Key key) const;
		//�ش� Ű�� �� ù �����ӿ��� true, �׿ܿ� false
		bool IsKeyReleased(Key key) const;

		//�����ִٸ� true, �׿ܿ� false
		bool IsButtonDown(MouseButton button) const;
		//�ش� ��ư�� ���� ù �����ӿ��� true, �׿ܿ� false
		bool IsButtonPressed(MouseButton button) const;
		//�ش� ��ư�� �� ù �����ӿ��� true, �׿ܿ� false
		bool IsButtonReleased(MouseButton button) const;

		//���콺 Ŀ�� ��ġ
		ivec2 GetCursorPos() const;
	private:
		static const int KEY_COUNT = 256;
		static const int KEY_ID_MIN = 0x07;
		static const int KEY_ID_MAX = 0xfe;

	private:
		Input();
		~Input();

		void UpdateKeyState();
		void UpdateMouseState();

		void SetMouseState(MouseButton button, bool isDown);

		BYTE* m_CurKeyState;
		BYTE* m_PreKeyState;

		bool m_CurMouseState[2];
		bool m_PreMouseState[2];

		friend class Window;
	};
}