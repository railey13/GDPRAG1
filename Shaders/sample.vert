#version 330 core //Version number

//Gets the data at Attrib Index 0
//Converts it and stores it into a Vec3

layout(location = 0) in vec3 aPos;

//creates a transform variable

uniform mat4 transform;

//Projection matrix
uniform mat4 projection;

void main(){
	//Multiplying the transformation matrix to
	//the vec4 version of aPos to get the final Pos
	
	gl_Position = projection * transform * vec4(aPos, 1.0);
} 