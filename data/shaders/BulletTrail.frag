
uniform sampler2D diffuseTexture;
uniform float time;


void main(void)
{
	// Determine how much time this texel has to live
	float leftOver = max(gl_TexCoord[0].x - time, 0.0) + 0.1;

	// Calculate the overall attenation over time
	float timeAttenuation = pow(1.0 - time, 5.0);

	// Apply attenation contribution from tex coordinate and time
	float alpha = min(leftOver / time, 1.0) * timeAttenuation;

	vec2 offsetVec = vec2( 0, 0 );

	// Exponential time will give the dissapation
	// effect a jump start.  At time = 1.0
	// expTime should also be close to 1.0
	float expTime = 1.0 - exp( -5.0 * time );

	// Decreasing the range pulls the texture coords
	// in resulting in a vertical stretching
	float finalRange = 0.5 - 0.8 * expTime;

	float percentage = ( gl_TexCoord[0].y - 0.5 ) / 0.5;
	float offset = finalRange * percentage;

	offsetVec = vec2( 0.0, offset );

	// Sample the texture
	vec4 diffuse = texture2D( diffuseTexture, gl_TexCoord[0].xy + offsetVec );
	vec4 zero    = vec4( 0, 0, 0, 0 );

	// Blend between black and the texture color
	gl_FragColor = mix( zero, diffuse, alpha);
	//gl_FragColor = vec4(time, time, time, 1.0);
}
