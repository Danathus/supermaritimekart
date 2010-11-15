

void main(void)
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	//setting gl_Position is the only requirement of the vertex shader
	//multiplying by gl_ModelViewProjectionMatrix moves the world or local coordinate
	//of this vertex into screen space
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}

