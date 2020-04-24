Instructions for how to use Material Editor .txt format:

Available Shader Programs:
	prog:draw-Phong-mul-fwd-mrt
	prog:draw-nonphoto-multi-mrt

Available Texture Types:
	color
	normal
	metallic
	roughness

Choose one of the above shader programs to use for this material (currently just lighting programs, multi-pass support in the the works)

Then, choose what textures you want to use. A texture declaration is one of the above texture types followed by the path to the texture. The texture must be placed somewhere inside of the 'resources' folder, such as 'resources/image.png' or 'resources/tex/earth/image.tga'.

Example given below:

shaderprogram
	prog:draw-Phong-mul-fwd-mrt
texture
	color tex/crate/crate4_low_lambert1_BaseColor.tga
texture
	normal tex/crate/crate4_low_lambert1_Normal.tga
texture
	metallic tex/crate/crate4_low_lambert1_Metallic.tga
texture
	roughness tex/crate/crate4_low_lambert1_Roughness.tga