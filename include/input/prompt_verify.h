#pragma once

#include "input/prompt_dynamic.h"

enum PromptVerifyResult
{
    PROMPT_OK = 0,  // Causes an exit with passed argument as result
    PROMPT_RETRY,   // Causes a retry of the prompt
    PROMPT_FAIL     // Causes an exit with a non-zero exit code
};

typedef enum PromptVerifyResult (*VerifyFunction)(const char* input);

struct PromptVerify
{
    PromptView result;
    VerifyFunction verifier;
};

struct PromptVerify* prompt_verify_init(
    size_t max_size, VerifyFunction verifier);
void prompt_verify_clean(struct PromptVerify* ctx);

int prompt_verify_text(struct PromptVerify* verify, const char* prompt_msg);
int prompt_verify_password(struct PromptVerify* verify, const char* prompt_msg);
