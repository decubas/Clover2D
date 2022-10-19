		
			#type vertex
			#version 330 core
			layout (location = 0) in vec3 a_Position;
			layout (location = 1) in vec2 a_TexCoord;
			layout (location = 2) in vec3 a_Normal;

			#define MAX_LIGHTS 4
			
			uniform mat4 u_vp_matrix;
			uniform mat4 u_model_matrix;
			uniform mat4 u_lights_vp_matrix[MAX_LIGHTS];

			out vec3 pos;

			out vec2 texCoord;
			out vec3 normal;
			out vec3 fragPos;
			out vec4 lightSpace[MAX_LIGHTS];
			
			void main()
			{
				fragPos = vec3(u_model_matrix * vec4(a_Position, 1.0));
				for(int i = 0; i < MAX_LIGHTS; i++)
				{
					lightSpace[i] = u_lights_vp_matrix[i] * vec4(fragPos, 1.0);
				}
				normal = a_Normal;
				texCoord = a_TexCoord;
				gl_Position = u_vp_matrix * vec4(fragPos, 1.0);
				pos = a_Position;
			}




			#type fragment
			#version 330 core
	
			layout (location = 0) out vec4 o_color;

			#define MAX_LIGHTS 4

		struct Light {
			float intensity;

			float cutOff;
			float outCutOff;
  
			float constant;
			float linear;
			float quadratic;

			vec3 direction;

			vec4 color;
  
			vec3 position;

		};  
		uniform Light lights[MAX_LIGHTS];

		//DEFAULT TEXTURES
			uniform sampler2D u_depth;
			uniform sampler2D u_depth_1;
			uniform sampler2D u_depth_2;
			uniform sampler2D u_depth_3;

		//USER
			uniform sampler2D u_diffuse; // diffuse
			uniform sampler2D u_specular; // specular
			uniform sampler2D u_normal; // normal

		// MATERIAL
			uniform float u_Alpha;
			uniform float u_Metallic;
			uniform float u_Shininess;
			uniform float u_Roughness;
			uniform vec3 u_LightUse;
			uniform vec3 u_EyePos;
			uniform vec3 u_ColorBase;
			uniform float u_Tiling;

			uniform float u_Ambient;
			uniform float u_Diffuse;
			uniform float u_Specular;

		//
			
			in vec4 lightSpace[MAX_LIGHTS];
			in vec3 pos;
			in vec2 texCoord;
			in vec3 normal;
			in vec3 fragPos;

			vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir, vec4 fragPosLightSpace, int lightIndex);
			vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 fragPosLightSpace);
			vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 fragPosLightSpace, int lightIndex);
			float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir, int lightIndex);

			vec3 dif_color;
			vec3 spec_color;

			vec3 depth_color;
			vec3 depth_color_1;
			vec3 depth_color_2;
			vec3 depth_color_3;

			void main()
			{
				//Correccion de Gamma
				vec3 viewDir = normalize(u_EyePos - fragPos);
				vec3 result = vec3(0, 0, 0);
				int i = 0;
				int j = 0;
				int k = 0;
				depth_color = vec3(texture(u_depth, texCoord));
				depth_color_1 = vec3(texture(u_depth_1, texCoord));
				depth_color_2 = vec3(texture(u_depth_2, texCoord));
				depth_color_3 = vec3(texture(u_depth_3, texCoord));

				dif_color = vec3(texture(u_diffuse, texCoord * u_Tiling)) * u_ColorBase;
				spec_color = vec3(texture(u_specular, texCoord * u_Tiling));
				vec3 normal_ = vec3(texture(u_normal, texCoord * u_Tiling));


				for(i = 0; i < u_LightUse.x; i++)
				{
					result += CalcDirLight(lights[i + j + k], normal_, viewDir, lightSpace[i + j + k], i + j + k);
				}
				for(j = 0; j < u_LightUse.y; j++)
				{
					result += CalcPointLight(lights[i + j + k], normal_, fragPos, viewDir, lightSpace[i + j + k]);
				}	
				for(k = 0; k < u_LightUse.z; k++)
				{
					result += CalcSpotLight(lights[i + j + k], normal_, fragPos, viewDir, lightSpace[i + j + k], i + j + k);
				}

				o_color = vec4(result, u_Alpha);
				float gamma = 2.2f;
				o_color.rgb = pow(o_color.rgb, vec3(1.0f/gamma));

			}

		vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir, vec4 fragPosLightSpace, int lightIndex)
		{
			vec3 lightDir = normalize(-light.direction);

			// diffuse 
			float diff = max(dot(normal, lightDir), 0.0);

			// specular 
			vec3 reflectDir = reflect(-lightDir, normal);
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess) * u_Roughness;
			
			// result
			vec3 ambient = u_Ambient * dif_color * 0.2f;
			vec3 diffuse = u_Diffuse * diff * dif_color;
			vec3 specular = u_Specular * spec * spec_color;

			vec3 lightColor = normalize(vec3(light.color));

			diffuse *=  light.intensity;
			specular *=  light.intensity;

			// calculate shadow
			float shadow = ShadowCalculation(fragPosLightSpace, normal, lightDir, lightIndex);       

			return  (ambient + shadow * (diffuse + specular)) * lightColor;
		}

		vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 fragPosLightSpace)
		{
			vec3 lightDir = normalize(light.position - fragPos);

			// diffuse 
			float diff = max(dot(normal, lightDir), 0.0);
			
			// specular 
			vec3 reflectDir = reflect(-lightDir, normal);
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess) * u_Roughness;
			
			// attenuation
			float distance = length(light.position - fragPos);
			float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
			
			// results
			vec3 ambient = u_Ambient * dif_color * 0.2f;
			vec3 diffuse = u_Diffuse *diff * dif_color;
			vec3 specular = u_Specular * spec * spec_color;
			ambient  *= attenuation;
			diffuse *= attenuation * light.intensity;
			specular *= attenuation * light.intensity;
			return (ambient + diffuse + specular) * light.color.rgb;
		}


		vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 fragPosLightSpace, int lightIndex)
		{
			vec3 lightDir = normalize(light.position - fragPos);

			// diffuse 
			float diff = max(dot(normal, lightDir), 0.0);

			// specular 
			vec3 reflectDir = reflect(-lightDir, normal);
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess) * u_Roughness;

			// attenuation
			float distance = length(light.position - fragPos);
			float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 
			
			// intensity
			float theta = dot(lightDir, normalize(-light.direction)); 
			float epsilon = light.cutOff - light.outCutOff;
			float intensity = clamp((theta - light.outCutOff) / epsilon, 0.0, 1.0);

			// results
			vec3 ambient = u_Ambient * dif_color * 0.2f;
			vec3 diffuse = u_Diffuse *diff * dif_color;
			vec3 specular = u_Specular * spec * spec_color;

			diffuse *= attenuation * intensity * light.intensity;
			specular *= attenuation * intensity * light.intensity;

			// calculate shadow
			float shadow = ShadowCalculation(fragPosLightSpace, normal, lightDir, lightIndex);       


			return (ambient + shadow * (diffuse + specular)) * light.color.rgb;
		}
	

		float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir, int lightIndex)
		{
			vec3 projCoords = (fragPosLightSpace.xyz / fragPosLightSpace.w) * 0.5 + 0.5;
			float shadow = 0.0f;
			

			if(projCoords.z > 1.0)
				return 1.0f;


			float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);


			if(lightIndex == 0)
			{
				vec2 texelSize = 1.0 / textureSize(u_depth, 0);
				for(int x = -1; x <= 1; ++x)
				{
					for(int y = -1; y <= 1; ++y)
					{
						//if numero de light
						float pcfDepth = texture(u_depth, projCoords.xy + vec2(x, y) * texelSize).r; 
						shadow += projCoords.z - bias > pcfDepth ? 0.5 : 1.0;        
					}    
				}

			} else if(lightIndex == 1)
			{
				vec2 texelSize = 1.0 / textureSize(u_depth_1, 0);
				for(int x = -1; x <= 1; ++x)
				{
					for(int y = -1; y <= 1; ++y)
					{
						//if numero de light
						float pcfDepth = texture(u_depth_1, projCoords.xy + vec2(x, y) * texelSize).r; 
						shadow += projCoords.z - bias > pcfDepth ? 0.5 : 1.0;        
					}    
				}

			} else if(lightIndex == 2)
			{
				vec2 texelSize = 1.0 / textureSize(u_depth_2, 0);
				for(int x = -1; x <= 1; ++x)
				{
					for(int y = -1; y <= 1; ++y)
					{
						//if numero de light
						float pcfDepth = texture(u_depth_2, projCoords.xy + vec2(x, y) * texelSize).r; 
						shadow += projCoords.z - bias > pcfDepth ? 0.5 : 1.0;        
					}    
				}

			} else if(lightIndex == 3)
			{
				vec2 texelSize = 1.0 / textureSize(u_depth_3, 0);
				for(int x = -1; x <= 1; ++x)
				{
					for(int y = -1; y <= 1; ++y)
					{
						//if numero de light
						float pcfDepth = texture(u_depth_3, projCoords.xy + vec2(x, y) * texelSize).r; 
						shadow += projCoords.z - bias > pcfDepth ? 0.5 : 1.0;        
					}    
				}
			}

			shadow /= 9.0;

			

			return shadow;
		}