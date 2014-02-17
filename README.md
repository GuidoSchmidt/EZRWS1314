# Real-time rendering
## Uni Koblenz
### Winter term 2013/2014

#### Project members
- (Matthias Bohleber)
- Andreas Meyer
- Markus Solbach
- Guido Schmidt

#### Structure
- **ext** holds all external libraries for windows
    * **inlcude** the c++-header files of the libraries are placed here
    * **lib** this is where the prebuild .lib files are placed 
* **resources** 
    * **scenes** 3D scene files are placed here, every file extension should get its own folder here (e.g. **blend** for all .blend files) 
    * **shader** Shader source code is placed in here 
    * **textures** all used texture should be in here 
    * **ui** UI-definitions (rml and css) and fonts go here
* **src** All .cpp and .h (.hpp) files are in here

#### Building
Use either cmake or camke-gui to generate your project files. Our cmake scripts currently work for windows and linux, mac os is not yet supported.
As a Linux user, please download all library-packages using your package manager.

#### Used Libraries
- **[OpenGL](http://www.opengl.org/)** (Graphics library)
- **[GLEW](http://glew.sourceforge.net/)** (OpenGL extension laoder)
- **[GLFW](http://www.glfw.org/)** (Context handling, creating windows) 
- **[Assimp](http://assimp.sourceforge.net/)** (3D file loader)
- **[Rocket](http://librocket.com/)** (Graphical user interface)
- **[SOIL](http://www.lonesock.net/soil.html)** (Small texture loading library)
- **[]()** (texture loading library for HDR-textures)

#### Blender Collada/DAE-Export
Currently we are importing 3D scenes in the collada format. Unfortunatelly, cameras and lights are not completly supported. If you use [blender](http://www.blender.org), make sure to have **the diffuse texture at the last texture-slot** for each texture. This is a known blender-dae-export bug.




