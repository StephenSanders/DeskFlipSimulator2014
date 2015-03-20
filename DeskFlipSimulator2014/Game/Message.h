#pragma once

#include <Engine/Types.h>
#include <Engine/Font.h>

extern void MessageUpdateAll();
extern void MessageRenderAll();

class Message
{
public:
	Message(wchar_t *message);
};
