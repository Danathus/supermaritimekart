// Note - This shader is not optimized.  It is layed out for clarity.

uniform sampler2D baseTexture;
uniform float osg_SimulationTime;

varying vec4 vColorAmbientEmissive;

varying vec3 vNormal;
varying vec3 vViewDir;

varying vec3 vPos;

void main(void)
{  
   vec4 baseColor = texture2D(baseTexture, gl_TexCoord[0].st);

   // normalize all of our incoming vectors
   vec3 normal = normalize(vNormal);
   vec3 lightDir = normalize(gl_LightSource[0].position.xyz);
   vec3 viewDir = normalize(vViewDir);
   vec3 reflectionDir = normalize(2.0 * dot(normal, lightDir) * normal - lightDir);

   float cosAngle = max(0.0, dot(normal, lightDir));
   float reflectionAngle =  dot(reflectionDir, viewDir);

   // Calculate the contributions from each shading component
   vec3 diffuseColor = cosAngle * vec3(gl_LightSource[0].diffuse.rgb);
   //vec3 specularColor = vec3(gl_LightSource[0].specular.rgb) * pow(max(0.0, reflectionAngle), 32.0);
   //vec3 specularColor = gl_FrontMaterial.specular * pow(max(0.0, reflectionAngle), 32.0);
   vec3 specularColor = vec3(1.0, 1.0, 1.0) * pow(max(0.0, reflectionAngle), 32.0);

   vec3 lightContrib = vColorAmbientEmissive + diffuseColor + specularColor;

   // compute final color with light and base color
   vec3 result = lightContrib * (vec3(baseColor));
 
   gl_FragColor = vec4(result, baseColor.a);
   //gl_FragColor = vec4(normal, 1.0);
   //gl_FragColor = vec4(specularColor, 1.0);
}
