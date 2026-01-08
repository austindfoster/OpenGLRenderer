I plan to use this project to learn OpenGL and eventually move on to learn the Vulkan API \(and maybe DirectX or Metal\). 

This will be built out to become a model editor and maybe add some sculpting functionality to see if I can get anywhere close to the performance of ZBrush considering Blender's implementation is so slow. 

Current features this renderer has implemented:

* Drawing meshes with a given list of vertices
* Importing premade models using the [Assimp (Asset Import) Library](https://github.com/assimp/assimp)
* Custom materials, shaders, shaders with textures for masks \(eg. Normal maps, Height maps, Diffuse maps, Specular maps, Roughness maps\)
* Moving the camera around with simple keyboard controls \(WASD + Q&E for up and down, Z&C for zooming & SHIFT for rotation\)
* Wireframe is toggleable and you can reset the camera position

The current implementation is very limited. I started this over Christmas break and was bound to a MacBook Air, so many of the dynamic library expectations will be biased towards that while I'm building out functionality. I'll get back to it eventually, probably. I have mostly been using [LearnOpenGL](https://learnopengl.com) as a jumping off point and building out from there, so shout out to a great resource.
