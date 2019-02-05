/*!
 * @file
 * @brief This file contains implemenation of phong vertex and fragment shader.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <assert.h>
#include <math.h>

#include <student/gpu.h>
#include <student/student_shader.h>
#include <student/uniforms.h>

/// \addtogroup shader_side Úkoly v shaderech
/// @{

void phong_vertexShader(GPUVertexShaderOutput *const      output,
                        GPUVertexShaderInput const *const input,
                        GPU const                         gpu) {
  //---------------------------------------
  //----získání projekční a view matice----
  //---------------------------------------
	Uniforms const uniformsHandle = gpu_getUniformsHandle(
		gpu);
	UniformLocation const viewMatrixLocation = getUniformLocation(
		gpu,
		"viewMatrix");

	Mat4 const*const viewMatrix = shader_interpretUniformAsMat4(
		uniformsHandle,
		viewMatrixLocation);
	UniformLocation const projectionMatrixLocation = getUniformLocation(
		gpu,
		"projectionMatrix");

	Mat4 const*const projMatrix = shader_interpretUniformAsMat4(
		uniformsHandle,
		projectionMatrixLocation);
  //-----------------------------------------------------------
  //----získání atributů o pozici voláním fce vs_interpret*----
  //-----------------------------------------------------------
	Vec3 const*const positionvec3 = vs_interpretInputVertexAttributeAsVec3(
		gpu,
		input,
		0);

	Vec3 const*const normalvec3 = vs_interpretInputVertexAttributeAsVec3(
		gpu,
		input,
		1);
  //-----------------------------------------------------------------------------
  //----vynásobení projekční matice a view matice a uložení do matice matmult----
  //-----------------------------------------------------------------------------
	Mat4 matmult;
	multiply_Mat4_Mat4(&matmult, projMatrix, viewMatrix);
  //-----------------------------------------------
  //----Vložení vektoru který je vec3D to vec4D----
  //-----------------------------------------------
	Vec4 posVect4;
	copy_Vec3Float_To_Vec4(&posVect4, positionvec3, 1.f);
  //---------------------------------------------------------------
  //----vynásobení násobené matice(projekční a view) s vectorem----
  //---------------------------------------------------------------
	multiply_Mat4_Vec4(
		&output->gl_Position,
		&matmult,
		&posVect4);
  //-----------------------------------------------------------
  //----získání atributů o pozici voláním fce vs_interpret*----
  //-----------------------------------------------------------
	Vec3 *const nextPos = vs_interpretOutputVertexAttributeAsVec3(
		gpu,
		output,
		0);

	Vec3 *const nextNormal = vs_interpretOutputVertexAttributeAsVec3(
		gpu,
		output,
		1);
  //--------------------------------------------------------------
  //----inicializace a nastavení dat v nové matici podle staré----
  //--------------------------------------------------------------
	init_Vec3(
		nextPos,
		positionvec3->data[0],
		positionvec3->data[1],
		positionvec3->data[2]);

	init_Vec3(
		nextNormal,
		normalvec3->data[0],
		normalvec3->data[1],
    normalvec3->data[2]);
}
float myMax(float a,float b)
{
  if (a >= b)
    return a;
  return b;
}
float myMin(float a,float b)
{
  if (a <= b)
    return a;
  return b;
}

float myClamp(float a, float b, float c) {
	if (a < c) {
		return myMax(a, b);
	}

	return myMin(a, c);
}

float myIntPow(float a, int b)
{
  if(b == 0)
  return 1;
  for(int i = 1;i<=b;i++)
  {
    a*=a;
  }
  return a;
}

void phong_fragmentShader(GPUFragmentShaderOutput *const      output,
                          GPUFragmentShaderInput const *const input,
                          GPU const                           gpu) {
  /// \todo Naimplementujte fragment shader, který počítá phongův osvětlovací
  /// model s phongovým stínováním.<br>
  /// <b>Vstup:</b><br>
  /// Vstupní fragment by měl v nultém fragment atributu obsahovat
  /// interpolovanou pozici ve world-space a v prvním
  /// fragment atributu obsahovat interpolovanou normálu ve world-space.<br>
  /// <b>Výstup:</b><br>
  /// Barvu zapište do proměnné color ve výstupní struktuře.<br>
  /// <b>Uniformy:</b><br>
  /// Pozici kamery přečtěte z uniformní proměnné "cameraPosition" a pozici
  /// světla přečtěte z uniformní proměnné "lightPosition".
  /// Zachovejte jména uniformních proměnný.
  /// Pokud tak neučiníte, akceptační testy selžou.<br>
  /// <br>
  /// Dejte si pozor na velikost normálového vektoru, při lineární interpolaci v
  /// rasterizaci může dojít ke zkrácení.
  /// Zapište barvu do proměnné color ve výstupní struktuře.
  /// Shininess faktor nastavte na 40.f
  /// Difuzní barvu materiálu nastavte podle normály povrchu.
  /// V případě, že normála směřuje kolmo vzhůru je difuzní barva čistě bílá.
  /// V případě, že normála směřuje vodorovně nebo dolů je difuzní barva čiště zelená.
  /// Difuzní barvu spočtěte lineární interpolací zelené a bíle barvy pomocí interpolačního parameteru t.
  /// Interpolační parameter t spočtěte z y komponenty normály pomocí t = y*y (samozřejmě s ohledem na negativní čísla).
  /// Spekulární barvu materiálu nastavte na čistou bílou.
  /// Barvu světla nastavte na bílou.
  /// Nepoužívejte ambientní světlo.<br>
  /// <b>Seznam funkcí, které jistě využijete</b>:
  ///  - shader_interpretUniformAsVec3()
  ///  - fs_interpretInputAttributeAsVec3()

  //------------------------------------------------
  //----získání pozice kamery a pozice osvetlení----
  //------------------------------------------------
  Uniforms const uniformsHandle = gpu_getUniformsHandle(
		gpu);

	UniformLocation const cameraPositionLocation = getUniformLocation(
		gpu,
		"cameraPosition");

	Vec3 const*const camVec3 = shader_interpretUniformAsVec3(
		uniformsHandle,
		cameraPositionLocation);

	UniformLocation const lightPositionLocation = getUniformLocation(
		gpu,
		"lightPosition");

	Vec3 const*const lightVec3 = shader_interpretUniformAsVec3(
		uniformsHandle,
		lightPositionLocation);
  //-----------------------------------------------------------
  //----získání atributů o pozici voláním fce vs_interpret*----
  //-----------------------------------------------------------
	Vec3 const*const vOut_p = fs_interpretInputAttributeAsVec3 (
		gpu,
		input,
		0);


	Vec3 const*const vOut_n = fs_interpretInputAttributeAsVec3 (
		gpu,
		input,
		1);

  //--------------------------------------------------------------------------------------------------------------------------------
  //----nastavení shininess, počítání s vektory normalizace, získání části vektoru(subvect), násobení,sečtení a nakonec obarvení----
  //--------------------------------------------------------------------------------------------------------------------------------
	Vec3 greenColor;
	init_Vec3(&greenColor, 0.f, 1.f, 0.f);

	Vec3 whiteColor;
	init_Vec3(&whiteColor, 1.f, 1.f, 1.f);

	float shininess = 40.f;

	Vec3 tmp;
	Vec3 tmp2;
	sub_Vec3(&tmp, lightVec3, vOut_p);

	Vec3 normalizedLocation;
	normalize_Vec3(&normalizedLocation, &tmp);

	Vec3 normalizedNormal;
	normalize_Vec3(&normalizedNormal, vOut_n);

	float deltaF = myClamp(dot_Vec3(&normalizedNormal, &normalizedLocation), 0.f, 1.f);

	sub_Vec3(&tmp, camVec3, vOut_p);

	Vec3 V;
	normalize_Vec3(&V, &tmp);

	multiply_Vec3_Float(&tmp, &V, -1.f);

	Vec3 reflexion;
	reflect(&reflexion, &tmp, &normalizedNormal);
	normalize_Vec3(&reflexion, &reflexion);

	float shininessF = myIntPow(myClamp(dot_Vec3(&reflexion, &normalizedLocation), 0.f, 1.f), (int)shininess);

	float dL = 1.f;
	float sL = 0.5f;

	Vec3 color;
	multiply_Vec3_Float(&tmp, &greenColor, deltaF*dL);
	multiply_Vec3_Float(&tmp2, &whiteColor, shininessF*sL);

	Vec3 tmpNormal;
	init_Vec3(&tmpNormal, 0.f, 1.f, 0.f);
  
	float alfa = ((tmpNormal.data[0]*normalizedNormal.data[0])+(tmpNormal.data[1]*normalizedNormal.data[1])+(tmpNormal.data[2]*normalizedNormal.data[2]));
	add_Vec3(&color, &tmp, &tmp2);
	mix_Vec3(&color, &color,&whiteColor,alfa);	//mixování barev interpolace podle alfy

	output->color.data[0] = color.data[0];
	output->color.data[1] = color.data[1];
	output->color.data[2] = color.data[2];
  output->color.data[3] = 1.f;
}

/// @}
