// Copyright 2017/2018 Joshua Ashton - Licensed under MIT
// https://github.com/Joshua-Ashton/Collision

#ifndef COLLISION_H
#define COLLISION_H

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef _WIN32
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
 #endif


#ifdef __cplusplus
extern "C"
{
#endif

	typedef void(*HandlerFunc)();

#define collision_error_handler __G4TdI4dj6cvkcBP5ZJk8
#define collision_success_handler __suOqLymSzxEaowziKT3b

#ifndef COLLISION_NO_FACADE_ARG
#define COLLISION_NO_FACADE_ARG "--nofacade"
#endif

#define COLLISION_CRASH \
	{ \
		int* crash_asdasdasd = (int*) 0; *crash_asdasdasd = 0xDEADED; \
	}

#define COLLISION_LAYER_WINDOWS( argc, argv ) \
	{ \
		STARTUPINFOA start_info = { sizeof(STARTUPINFOA) }; \
		PROCESS_INFORMATION process_info = { 0 }; \
		\
		size_t size = 0; \
		for (int i = 0; i < new_argc; i++) \
			size += strlen(new_argv[i]); \
		size += new_argc; \
		char* args_string = (char*) malloc(size); \
		args_string[0] = '\0'; \
		for (int i = 0; i < new_argc; i++) \
			snprintf(args_string + strlen(args_string), size - strlen(args_string), "%s%s", new_argv[i], i != new_argc - 1 ? " " : ""); \
		\
		if ( !CreateProcessA( (const char*) new_argv[0], args_string, NULL, NULL, FALSE, 0, NULL, current_path, &start_info, &process_info ) ) \
			COLLISION_CRASH; \
		\
		WaitForSingleObject( process_info.hProcess, INFINITE ); \
		\
		DWORD exit_code = 42; \
		GetExitCodeProcess( process_info.hProcess, &exit_code ); \
		\
		if ( exit_code != 0 && exit_code != 1 ) \
		{ \
			if ( collision_error_handler ) \
				collision_error_handler(); \
		} \
		else \
			if ( collision_success_handler ) \
				collision_success_handler(); \
		\
		CloseHandle(process_info.hProcess); \
	}

#define COLLISION_NONFACADE_INIT_WINDOWS \
	SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);

#ifdef _WIN32
#define COLLISION_LAYER( argc, argv ) COLLISION_LAYER_WINDOWS( argc, argv )
#define COLLISION_NONFACADE_INIT COLLISION_NONFACADE_INIT_WINDOWS
#endif

#define COLLISION_ENABLE( fail, success, argc, argv ) \
	{ \
		HandlerFunc collision_error_handler = ( HandlerFunc ) fail; \
		HandlerFunc collision_success_handler = ( HandlerFunc ) success; \
		int facade = strcmp( argv[argc - 1], COLLISION_NO_FACADE_ARG ); \
		char current_path[FILENAME_MAX]; \
		snprintf( current_path, FILENAME_MAX, "%s", argv[0] ); \
		GetCurrentDir( current_path, FILENAME_MAX ); \
		if ( facade != 0 ) \
		{ \
			char** new_argv = (char**) malloc( sizeof(char*) * (argc + 1) ); \
			int new_argc; \
			for (new_argc = 0; new_argc < argc; new_argc++) \
				new_argv[new_argc] = argv[new_argc]; \
			new_argv[new_argc++] = COLLISION_NO_FACADE_ARG; \
			COLLISION_LAYER( argc, argv ); \
			free(new_argv); \
			return 0; \
		} \
		else \
		{ \
			COLLISION_NONFACADE_INIT \
		} \
	}

#ifdef __cplusplus
}
#endif

#endif