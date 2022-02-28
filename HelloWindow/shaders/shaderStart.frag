#version 410 core

in vec3 fNormal;
in vec4 fPosEye;
in vec2 fTexCoords;
in vec4 fragPosLightSpace;
in vec3 fragPos;

out vec4 fColor;

//lighting
uniform	vec3 lightDir;
uniform	vec3 lightColor;
uniform	vec3 lightDir1;
uniform	vec3 lightColor1;
uniform	vec3 lightDir2;
uniform	vec3 lightColor2;
uniform	vec3 lightDir3;
uniform	vec3 lightColor3;
uniform	vec3 lightDir4;
uniform	vec3 lightColor4;
uniform	vec3 lightDir5;
uniform	vec3 lightColor5;
uniform	vec3 lightDir6;
uniform	vec3 lightColor6;
uniform	vec3 lightDir7;
uniform	vec3 lightColor7;
uniform	vec3 lightDir8;
uniform	vec3 lightColor8;
uniform	int turnOnLight;

//texture
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D shadowMap;

vec3 ambient;
float ambientStrength = 0.2f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.5f;
float shininess = 32.0f;

//for second light source
float ambientStrength1 = 100.0f;
float specularStrength1 = 30.0f;
float constant = 1.0f;
float linear = 0.7f;
float quadratic = 1.8f;

float ambientStrength2 = 500.0f;
float specularStrength2 = 150.0f;
float constant2 = 1.0f;
float linear2 = 0.7f;
float quadratic2 = 1.8f;

float computeShadow()
{
	vec3 normalizedCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;


	// Transform to [0,1] range
	normalizedCoords = normalizedCoords * 0.5 + 0.5;

	//Get closest depth value from light's perspective
	float closestDepth = texture(shadowMap, normalizedCoords.xy).r;

	// Get depth of current fragment from light's perspective
	float currentDepth = normalizedCoords.z;

	// Check whether current frag pos is in shadow
	float bias = max(0.05f * (1.0f - dot(fNormal, lightDir)), 0.005f);
	//float shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;
	float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, normalizedCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(normalizedCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;

}

void computeLightComponents()
{		
	vec3 cameraPosEye = vec3(0.0f);//in eye coordinates, the viewer is situated at the origin
	
	//transform normal
	vec3 normalEye = normalize(fNormal);	
	
	//compute light direction
	vec3 lightDirN = normalize(lightDir);
	
	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fPosEye.xyz);
		
	//compute ambient light
	ambient = ambientStrength * lightColor;
	
	//compute diffuse light
	diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;
	
	//compute specular light
	vec3 reflection = reflect(-lightDirN, normalEye);
	float specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
	specular = specularStrength * specCoeff * lightColor;

	if(turnOnLight==1){
		//for second source of light
		vec3 LightPos1 = normalize(lightDir1 - fragPos);
		vec3 refDir1 = reflect(-LightPos1, normalEye);
		specCoeff = pow(max(dot(viewDirN, refDir1), 0.0f), shininess);
		float dist = length(lightDir1 - fragPos);
		float att = 1.0f / (constant + linear * dist + quadratic * (dist * dist));
		ambient += att * ambientStrength1 * lightColor1;
		diffuse += att * max(dot(normalEye, LightPos1), 0.0f)* lightColor1;
		specular += att * specularStrength1 * specCoeff * lightColor1;

		//for third source of light
		vec3 LightPos2 = normalize(lightDir2 - fragPos);
		vec3 refDir2 = reflect(-LightPos2, normalEye);
		specCoeff = pow(max(dot(viewDirN, refDir2), 0.0f), shininess);
		float dist2 = length(lightDir2 - fragPos);
		float att2 = 1.0f / (constant + linear * dist2 + quadratic * (dist2 * dist2));
		ambient += att2 * ambientStrength1 * lightColor2;
		diffuse += att2 * max(dot(normalEye, LightPos2), 0.0f)* lightColor2;
		specular += att2 * specularStrength1 * specCoeff * lightColor2;

		//for fourth source of light
		vec3 LightPos3 = normalize(lightDir3 - fragPos);
		vec3 refDir3 = reflect(-LightPos3, normalEye);
		specCoeff = pow(max(dot(viewDirN, refDir3), 0.0f), shininess);
		float dist3 = length(lightDir3 - fragPos);
		float att3 = 1.0f / (constant + linear * dist3 + quadratic * (dist3 * dist3));
		ambient += att3 * ambientStrength1 * lightColor3;
		diffuse += att3 * max(dot(normalEye, LightPos3), 0.0f)* lightColor3;
		specular += att3 * specularStrength1 * specCoeff * lightColor3;

		//for five source of light
		vec3 LightPos4 = normalize(lightDir4 - fragPos);
		vec3 refDir4 = reflect(-LightPos4, normalEye);
		specCoeff = pow(max(dot(viewDirN, refDir4), 0.0f), shininess);
		float dist4 = length(lightDir4 - fragPos);
		float att4 = 1.0f / (constant + linear * dist4 + quadratic * (dist4 * dist4));
		ambient += att4 * ambientStrength1 * lightColor4;
		diffuse += att4 * max(dot(normalEye, LightPos4), 0.0f)* lightColor4;
		specular += att4 * specularStrength1 * specCoeff * lightColor4;

		//for six source of light
		vec3 LightPos5 = normalize(lightDir5 - fragPos);
		vec3 refDir5 = reflect(-LightPos5, normalEye);
		specCoeff = pow(max(dot(viewDirN, refDir5), 0.0f), shininess);
		float dist5 = length(lightDir5 - fragPos);
		float att5 = 1.0f / (constant + linear * dist5 + quadratic * (dist5 * dist5));
		ambient += att5 * ambientStrength1 * lightColor5;
		diffuse += att5 * max(dot(normalEye, LightPos5), 0.0f)* lightColor5;
		specular += att5 * specularStrength1 * specCoeff * lightColor5;

		//for seven source of light
		vec3 LightPos6 = normalize(lightDir6 - fragPos);
		vec3 refDir6 = reflect(-LightPos6, normalEye);
		specCoeff = pow(max(dot(viewDirN, refDir6), 0.0f), shininess);
		float dist6 = length(lightDir6 - fragPos);
		float att6 = 1.0f / (constant2 + linear2 * dist6 + quadratic2 * (dist6 * dist6));
		ambient += att6 * ambientStrength2 * lightColor6;
		diffuse += att6 * max(dot(normalEye, LightPos6), 0.0f)* lightColor6;
		specular += att6 * specularStrength2 * specCoeff * lightColor6;

		//for eight source of light
		vec3 LightPos7 = normalize(lightDir7 - fragPos);
		vec3 refDir7 = reflect(-LightPos7, normalEye);
		specCoeff = pow(max(dot(viewDirN, refDir7), 0.0f), shininess);
		float dist7 = length(lightDir7 - fragPos);
		float att7 = 1.0f / (constant + linear2 * dist7 + quadratic2 * (dist7 * dist7));
		ambient += att7 * ambientStrength2 * lightColor7;
		diffuse += att7 * max(dot(normalEye, LightPos7), 0.0f)* lightColor7;
		specular += att7 * specularStrength2 * specCoeff * lightColor7;

		//for six source of light
		vec3 LightPos8 = normalize(lightDir8 - fragPos);
		vec3 refDir8 = reflect(-LightPos8, normalEye);
		specCoeff = pow(max(dot(viewDirN, refDir8), 0.0f), shininess);
		float dist8 = length(lightDir8 - fragPos);
		float att8 = 1.0f / (constant + linear2 * dist8 + quadratic2 * (dist8 * dist8));
		ambient += att8 * ambientStrength2 * lightColor8;
		diffuse += att8 * max(dot(normalEye, LightPos8), 0.0f)* lightColor8;
		specular += att8 * specularStrength2 * specCoeff * lightColor8;
	}
}

float computeFog()
{
	float fogDensity = 0.05f;
	float fragmentDistance = length(fPosEye);
	float fogFactor = exp(-pow(fragmentDistance * fogDensity, 4));

	return clamp(fogFactor, 0.8f, 1.0f);
}

void main() 
{
	computeLightComponents();
	float fogFactor = computeFog();

	vec3 baseColor = vec3(0.9f, 0.35f, 0.0f);//orange
	
	ambient *= texture(diffuseTexture, fTexCoords).rgb;
	diffuse *= texture(diffuseTexture, fTexCoords).rgb;
	specular *= texture(specularTexture, fTexCoords).rgb;
	
    float shadow = computeShadow();
	vec3 color = min((ambient + (1.0f - shadow)*diffuse) + (1.0f - shadow)*specular, 1.0f);
    vec4 fogColor;
	fogColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
    vec4 aux = vec4(color, 1.0f);

    fColor = mix(fogColor, aux, fogFactor);
    //fColor = vec4(color, 1.0f);
}
