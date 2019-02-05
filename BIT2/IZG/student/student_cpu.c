/*!
 * @file
 * @brief This file contains implementation of cpu side for phong shading.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 *
 */

#include <assert.h>
#include <math.h>

#include <student/buffer.h>
#include <student/bunny.h>
#include <student/camera.h>
#include <student/gpu.h>
#include <student/linearAlgebra.h>
#include <student/mouseCamera.h>
#include <student/student_cpu.h>
#include <student/student_pipeline.h>
#include <student/student_shader.h>
#include <student/swapBuffers.h>
#include <student/uniforms.h>
#include <student/vertexPuller.h>

#include <student/globals.h>

/**
 * @brief This structure contains all global variables for this method.
 */
struct PhongVariables {
  GPU gpu;
	///This variable contains light poistion in world-space.
	Vec3 lightPosition;
	ProgramID program;
	VertexPullerID vertPuller;
	BufferID vbo;
  BufferID ebo;
} phong;  ///<instance of all global variables for triangle example.

/// \addtogroup cpu_side Úkoly v cpu části
/// @{

void phong_onInit(int32_t width, int32_t height) {

  
  // create gpu
  phong.gpu = cpu_createGPU();
  // set viewport size
  cpu_setViewportSize(phong.gpu, (size_t)width, (size_t)height);
  // init matrices
  cpu_initMatrices(width, height);
  // init lightPosition
  init_Vec3(&phong.lightPosition, 1000.f, 1000.f, 1000.f);
  phong.program = cpu_createProgram(phong.gpu);
  //------------------------------
  //------vytvoření bufferů-------
  //------------------------------
	cpu_createBuffers(
		phong.gpu,
		2,
		&phong.vbo);

	cpu_createBuffers(
		phong.gpu,
		1,
		&phong.ebo);
  //-----------------------------------
  //----nastavení velikosti bufferů----
  //-----------------------------------
	cpu_bufferData(
		phong.gpu,
		phong.vbo,
		sizeof(bunnyVertices),
		bunnyVertices);

	cpu_bufferData(
		phong.gpu,
		phong.ebo,
		sizeof(bunnyIndices),
		bunnyIndices);
  //--------------------------------------------------------------------------
  //----vytvoření a nastavení vertex pulleru (prvního prvku v render pipe)----
  //--------------------------------------------------------------------------
	cpu_createVertexPullers(
		phong.gpu,
		1,
		&phong.vertPuller);

	cpu_setVertexPullerHead(
		phong.gpu,
		phong.vertPuller,
		0,
		phong.vbo,
		sizeof(float) * 0,
		sizeof(float) * 6
	);

	cpu_setVertexPullerHead(
		phong.gpu,
		phong.vertPuller,
		1,
		phong.vbo,
		sizeof(float) * 3,
		sizeof(float) * 6
	);

	cpu_enableVertexPullerHead(
		phong.gpu,
		phong.vertPuller,
		0);

	cpu_enableVertexPullerHead(
		phong.gpu,
		phong.vertPuller,
		1);

	cpu_setIndexing(
		phong.gpu,
		phong.vertPuller,
		phong.ebo,
    sizeof(float));
  //------------------------------------------
  //----připojení vertex shaderu k pulleru----
  //------------------------------------------
	cpu_attachVertexShader(
		phong.gpu,
		phong.program,
		phong_vertexShader);
  //pripnutí fragment shaderu
	cpu_attachFragmentShader(
		phong.gpu,
		phong.program,
		phong_fragmentShader);
  //-----------------------------
  //----nastavení interpolace----
  //-----------------------------
	cpu_setAttributeInterpolation(
		phong.gpu,
		phong.program,
		0,
		ATTRIB_VEC3, SMOOTH);
	cpu_setAttributeInterpolation(
		phong.gpu,
		phong.program,
		1,
		ATTRIB_VEC3, SMOOTH);
  //------------------------------------------
  //-----rezervace uniformních proměnných-----
  //------------------------------------------
  //rezervace vectoru3D pro kameru
	cpu_reserveUniform(
		phong.gpu,
		"cameraPosition",
		UNIFORM_VEC3);
  //rezervace vectoru3D pro osvícení
	cpu_reserveUniform(
		phong.gpu,
		"lightPosition",
		UNIFORM_VEC3);
  //rezervace view matice
	cpu_reserveUniform(
		phong.gpu,
		"viewMatrix",
		UNIFORM_MAT4);
  //rezervace projekční matice
  cpu_reserveUniform(
		phong.gpu,
		"projectionMatrix",
		UNIFORM_MAT4);


}

/// @}

void phong_onExit() { cpu_destroyGPU(phong.gpu); }
/// \addtogroup cpu_side
/// @{

void phong_onDraw(SDL_Surface* surface) {
  assert(surface != NULL);

  // clear depth buffer
  cpu_clearDepth(phong.gpu, +INFINITY);
  Vec4 color;
  init_Vec4(&color, .1f, .1f, .1f, 1.f);
  // clear color buffer
  cpu_clearColor(phong.gpu, &color);

  // aktivace shader programu
	cpu_useProgram(
		phong.gpu,
		phong.program);
  //aktivace vertex pulleru
	cpu_bindVertexPuller(
		phong.gpu,
		phong.vertPuller);
  //-----------------------------------------------------------
  //-----načtení všech dat do všech uniformních proměnných-----
  //-----------------------------------------------------------
	UniformLocation const viewMatrixUniform = getUniformLocation(
		phong.gpu,
		"viewMatrix");

	cpu_uniformMatrix4fv(
		phong.gpu,
		viewMatrixUniform,
		(float*)&viewMatrix);

	UniformLocation const projectionMatrixUniform = getUniformLocation(
		phong.gpu,
		"projectionMatrix");

	cpu_uniformMatrix4fv(
		phong.gpu,
		projectionMatrixUniform,
		(float*)&projectionMatrix);

	UniformLocation const cameraPositionUniform = getUniformLocation(
		phong.gpu,
		"cameraPosition");

	cpu_uniform3f(
		phong.gpu,
		cameraPositionUniform,
		cameraPosition.data[0],
		cameraPosition.data[1],
		cameraPosition.data[2]);

	UniformLocation const lightPositionUniform = getUniformLocation(
		phong.gpu,
		"lightPosition");

	cpu_uniform3f(
		phong.gpu,
		lightPositionUniform,
		phong.lightPosition.data[0],
		phong.lightPosition.data[1],
		phong.lightPosition.data[2]);

  //------------------------------
  //----vykreslení tojůhelníků----
  //------------------------------
	cpu_drawTriangles(
		phong.gpu,
2092 * 3);

  // copy image from gpu to SDL surface
  cpu_swapBuffers(surface, phong.gpu);
}

/// @}
