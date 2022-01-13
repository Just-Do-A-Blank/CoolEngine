#pragma once

#if _DEBUG 
#define LOG(message)cout<<message<<endl;
#else
#define LOG(message)
#endif
