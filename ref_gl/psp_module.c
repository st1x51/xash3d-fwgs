#include <pspkernel.h>
#include <GL/gl.h>

PSP_MODULE_INFO("ref_gl", PSP_MODULE_USER, 1, 0);

int module_start( SceSize arglen, void *argp )
{
	return 0;
}

int module_stop( SceSize arglen, void *argp )
{
	return 0;
}

void glHint( GLenum target, GLenum mode )
{

}

static GLboolean fog_enabled = GL_TRUE;

GLboolean glIsEnabled( GLenum cap )
{
	return fog_enabled;
}

void glClipPlane( GLenum plane, const GLdouble *equation )
{
}

void glTexImage1D( GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels )
{
	glTexImage2D( GL_TEXTURE_2D, level, internalformat, width, 1, border, format, type, pixels );
}

void glTexImage3D( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels )
{
	glTexImage2D( GL_TEXTURE_2D, level, internalformat, width, height, border, format, type, pixels );
}

void glTexSubImage1D( GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels )
{
	glTexSubImage2D( target, level, xoffset, 0, width, 1, format, type, pixels );
}

void glTexSubImage3D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels )
{
	glTexSubImage2D( target, level, xoffset, yoffset, width, height, format, type, pixels );
}

void glPointSize( GLfloat size )
{

}

void glDebugMessageControlARB( GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled )
{

}

void glDebugMessageCallbackARB(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{

}