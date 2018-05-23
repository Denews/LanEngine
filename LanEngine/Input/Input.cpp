#include "Defines.h"
#include "Input.h"
#include "../Windows/Window.h"

namespace Lan
{
	Input::Input() :
		m_CurKeyState(new BYTE[KEY_COUNT]),
		m_PreKeyState(new BYTE[KEY_COUNT])
	{

	}

	Input::~Input()
	{
		if (m_CurKeyState)
		{
			delete[] m_CurKeyState;
		}

		if (m_PreKeyState)
		{
			delete[] m_PreKeyState;
		}
	}

	void Input::update()
	{
		updateKeyState();
		updateMouseState();
	}

	void Input::updateKeyState()
	{
		//���� ���¸� ���� �������� ���·� ��ȯ
		BYTE* temp = m_PreKeyState;
		m_PreKeyState = m_CurKeyState;
		m_CurKeyState = temp;

		//���� �������� Ű���� ���� Ȯ��
		if (!GetKeyboardState(m_CurKeyState))
		{
			LOG(LogLevel::Error, "Ű���� �Է� ����");
		}
	}

	void Input::updateMouseState()
	{
		//���� ���¸� ���� �������� ���·� ��ȯ
		m_PreKeyState[0] = m_CurMouseState[0];
		m_PreKeyState[1] = m_CurMouseState[1];
	}

	void Input::setMouseState(MouseButton button, bool isDown)
	{
		switch (button)
		{
		case MouseButton::Left:
			m_CurMouseState[0] = isDown;
			break;
		case MouseButton::Right:
			m_CurMouseState[1] = isDown;
			break;
		}
	}


	bool Input::isKeyDown(Key key) const
	{
		if (key <= KEY_ID_MIN || KEY_ID_MAX < key)
		{
			LOG(LogLevel::Warning, "�߸��� Ű���� Ű�� üũ");
			return false;
		}

		return m_CurKeyState[key] & 0xf0;
	}

	bool Input::isKeyPressed(Key key) const
	{
		if (key <= KEY_ID_MIN || KEY_ID_MAX < key)
		{
			LOG(LogLevel::Warning, "�߸��� Ű���� Ű�� üũ");
			return false;
		}

		if (!(m_PreKeyState[key] & 0xf0) && (m_CurKeyState[key] & 0xf0))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Input::isKeyReleased(Key key) const
	{
		if (key <= KEY_ID_MIN || KEY_ID_MAX < key)
		{
			LOG(LogLevel::Warning, "�߸��� Ű���� Ű�� üũ");
			return false;
		}

		if ((m_PreKeyState[key] & 0xf0) && !(m_CurKeyState[key] & 0xf0))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Input::isButtonDown(MouseButton button) const
	{
		switch (button)
		{
			case MouseButton::Left:
				return m_CurMouseState[0];
			case MouseButton::Right:
				return m_CurMouseState[1];
		}
		return false;
	}

	bool Input::isButtonPressed(MouseButton button) const
	{
		switch (button)
		{
		case MouseButton::Left:
			return !m_PreMouseState[0] && m_CurMouseState[0];
		case MouseButton::Right:
			return !m_PreMouseState[1] && m_CurMouseState[1];
		}
		return false;
	}

	bool Input::isButtonReleased(MouseButton button) const
	{
		switch (button)
		{
		case MouseButton::Left:
			return m_PreMouseState[0] && !m_CurMouseState[0];
		case MouseButton::Right:
			return m_PreMouseState[1] && !m_CurMouseState[1];
		}
		return false;
	}

	ivec2 Input::getCursorPos() const
	{
		POINT cursorPos;
		if (!GetCursorPos(&cursorPos))
		{
			LOG(LogLevel::Error, "Ŀ�� ��ġ ���� ����");
		}

		ScreenToClient(Window::getInstance().getHandle(), &cursorPos);

		return { cursorPos.x, cursorPos.y };
	}
}