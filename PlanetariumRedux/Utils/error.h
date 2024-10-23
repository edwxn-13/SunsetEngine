#pragma once


void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userparam) 
{

	if (type == GL_DEBUG_TYPE_ERROR) 
	{
		fprintf(stderr, "DebugCallback : Error - %!\n", message);
	}
}

void GLAPIENTRY DebguMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:					
		fprintf(stderr, "GL DebguMessageCallback: type = %s", "ERROR");					
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
	case GL_DEBUG_TYPE_PORTABILITY:
	case GL_DEBUG_TYPE_PERFORMANCE:
	case GL_DEBUG_TYPE_OTHER:					return;
	}

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_LOW:					fprintf(stderr, " severity = LOW ");											break;
	case GL_DEBUG_SEVERITY_MEDIUM:				fprintf(stderr, " severity = MEDIUM ");											break;
	case GL_DEBUG_SEVERITY_HIGH:				fprintf(stderr, " severity = HIGH ");											break;
	}

	fprintf(stderr, "message = % s\n", message);
}
