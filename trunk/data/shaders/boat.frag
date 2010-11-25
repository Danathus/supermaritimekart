uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D noiseTexture;

uniform int mode;
uniform float health; // 0.0 - 1.0

uniform float osg_SimulationTime;

varying vec3 vNormal;
varying vec3 vViewDir;
varying vec3 vLightDir;

varying vec3 scaledPosition;

float saturate(float inValue)
{
   return clamp(inValue, 0.0, 1.0);
}

float getDamage()
{
   vec4 noiseVec = texture2D(noiseTexture, gl_TexCoord[0].st);
   float noiseSum = noiseVec.x + noiseVec.y + noiseVec.z;

   return max(noiseSum - pow(health, 0.75), 0.0);
}

void main(void)
{  
   float damage = getDamage();

   if (damage > 1.35)
   {
      discard;
   }
   else if (damage > 1.3)
   {
      gl_FragColor = vec4(1.0, 0.2, 0.2, 1.0);
   }
   else 
   {
      vec4 baseColor = texture2D(diffuseTexture, gl_TexCoord[0].st);
      vec4 normalColor = texture2D(normalTexture, gl_TexCoord[0].st);
      //vec4 noiseColor = texture2D(noiseTexture, gl_TexCoord[0].st);
   
      vec3 normal = normalize((normalColor.xyz * 2.0) - 1.0);

      // normalize all of our incoming vectors
      vec3 lightDir = normalize(vLightDir);
      vec3 viewDir = normalize(vViewDir);
      vec3 reflectionDir = normalize(2.0 * dot(normal, lightDir) * normal - lightDir);

      float NdotL = saturate(dot(normal, lightDir));
      float reflectionAngle =  dot(reflectionDir, viewDir);

      // Calculate the contributions from each shading component
      vec3 ambientColor = vec3(gl_LightSource[0].ambient.rgb);
      vec3 diffuseColor = NdotL * vec3(gl_LightSource[0].diffuse.rgb);
      //vec3 specularColor = NdotL * vec3(gl_LightSource[0].specular.rgb) * pow(max(0.0, reflectionAngle), 128.0);
      vec3 specularColor = NdotL * vec3(1.0, 1.0, 1.0) * pow(max(0.0, reflectionAngle), 32.0);
      specularColor = clamp(specularColor, 0.0, normalColor.a);

   vec3 result = (ambientColor + diffuseColor) * baseColor + specularColor;  
   //vec3 test = vec3(pow(1.0 - dot(normal, viewDir), 6.0), 0.0, 0.0);
   gl_FragColor = vec4(result, baseColor.a);
   //gl_FragColor = vec4(test, 1.0);
   //gl_FragColor = vec4(specularColor.rgb, baseColor.a);
   //gl_FragColor = vec4(vec3(normalColor.a), 1.0);
   //gl_FragColor = vec4(noiseColor.rgb, 1.0);
   //gl_FragColor = vec4(health, 0.0, 0.0, 1.0);

  } 
   }
