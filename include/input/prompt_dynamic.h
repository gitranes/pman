#pragma once

#include "common/buffer_view.h"

// Use StringView's constructors and destructors
typedef struct StringView PromptView;

int prompt_view_text(PromptView* view, const char* prompt_msg);
int prompt_view_password(PromptView* view, const char* prompt_msg);
