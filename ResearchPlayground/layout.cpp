#include "layout.h"

// initial static member
KLWindowLayoutManager* KLWindowLayoutManager::s_Instance = nullptr;
std::once_flag KLWindowLayoutManager::s_OnceFlag;
