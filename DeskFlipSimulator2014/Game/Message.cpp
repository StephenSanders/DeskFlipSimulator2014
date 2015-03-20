#include "Message.h"
#include <Engine/Hash.h>
#include <Engine/Time.h>
#include <Engine/Platform.h>
#include <Engine/Debug.h>

#include <list>

struct MessageNode
{
	wchar_t *text;
	float timeLeft;
	float yPosition;
	float yTarget;
};

static std::list<MessageNode> messages;

void MessageUpdateAll()
{
	float yTarget = Platform::GetHeight() - 48.0f;
	for (std::list<MessageNode>::iterator it = messages.begin(); it != messages.end(); ++it) {
		it->timeLeft -= Time::Delta();
		it->yTarget = yTarget;

		if (it->timeLeft < 0.0f) {
			it->yTarget = Platform::GetHeight() + 48.0f;
		}

		if (it->yPosition < it->yTarget) {
			it->yPosition += (100.0f + abs(yTarget - it->yPosition)) * Time::Delta();
		}
		if (it->yPosition > it->yTarget) {
			it->yPosition = it->yTarget;
		}

		yTarget -= 32.0f;
	}

	if (!messages.empty() && messages.front().yPosition >= Platform::GetHeight() + 40.0f) {
		messages.pop_front();
	}
}

void MessageRenderAll()
{
	Font *font = new Font(L"comic_sans_16.spritefont", DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
	float offset = 0;
	for (auto i : messages) {
		font->SetColor(DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 0.3f));
		font->Draw(DirectX::XMFLOAT2(32.0f, i.yPosition), i.text);
		offset += 32.0f;
	}
}

Message::Message(wchar_t *message)
{
	MessageNode node;
	node.text = message;
	node.timeLeft = 5.0f;
	node.yPosition = -16.0f;
	node.yTarget = -16.0f;
	messages.push_back(node);
}
