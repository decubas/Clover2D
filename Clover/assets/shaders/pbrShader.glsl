		
			#type vertex
			#version 330 core
			layout (location = 0) in vec3 a_Position;
			layout (location = 1) in vec2 a_TexCoord;
			layout (location = 2) in vec3 a_Normal;
			layout (location = 3) in vec4 a_Color;

			#define MAX_LIGHTS 4
			
			uniform mat4 u_vp_matrix;
			uniform mat4 u_model_matrix;
			uniform mat4 u_lights_vp_matrix[MAX_LIGHTS];
			out vec4 color;
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

			mat4 vp_matrix;
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
			uniform sampler2D u_rough; // rough
			uniform sampler2D u_normal; // normal
			uniform sampler2D u_ambient; // ambient occlusion

		//Material
			uniform float u_Alpha;
			uniform float u_Tiling;
			uniform vec3  u_ColorBase;
			uniform float u_Metallic;
			uniform float u_Roughness;
			uniform float u_AmbientOcclusion;


			uniform vec3 u_LightUse;
			uniform vec3 u_EyePos;

		//IN VALUES

			in vec4 lightSpace[MAX_LIGHTS];
			in vec3 pos;
			in vec2 texCoord;
			in vec3 normal;
			in vec4 color;
			in vec3 fragPos;

			vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir, vec4 fragPosLightSpace, int lightIndex);
			vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 fragPosLightSpace);
			vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 fragPosLightSpace, int lightIndex);

			float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir, int lightIndex);


			vec3 getNormalFromMap();
			float DistributionGGX(vec3 N, vec3 H, float roughness);
			float GeometrySchlickGGX(float NdotV, float roughness);
			float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
			vec3 fresnelSchlick(float cosTheta, vec3 F0);

			const float PI = 3.14159265359f;

			vec3 depth_color;
			vec3 depth_color_1;
			vec3 depth_color_2;
			vec3 depth_color_3;

			vec3 dif_color;
			vec3 spec_color;
			float roughness;
			float ao;
			float metallic;

			vec3 F0;
			vec3 Lo;

			void main()
			{
				//Correccion de Gamma
				vec3 viewDir = normalize(u_EyePos - fragPos);
				vec3 result = vec3(0, 0, 0);
				int i = 0;
				int j = 0;
				int k = 0;
				depth_color       = vec3(texture(u_depth, texCoord));
				depth_color_1     = vec3(texture(u_depth_1, texCoord));
				depth_color_2     = vec3(texture(u_depth_2, texCoord));
				depth_color_3     = vec3(texture(u_depth_3, texCoord));

				dif_color       = vec3(texture(u_diffuse, texCoord * u_Tiling)) * u_ColorBase;
				spec_color      = vec3(texture(u_specular, texCoord * u_Tiling).r * u_Metallic);
				roughness		= texture(u_rough, texCoord * u_Tiling).r * u_Roughness;
				ao				= texture(u_ambient, texCoord * u_Tiling).r * u_AmbientOcclusion;
				vec3 normal_	= getNormalFromMap();

				metallic = spec_color.r;

				F0 = vec3(0.04f); 
				F0 = mix(F0, dif_color, metallic);

				Lo = vec3(0.0f);

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

				result += vec3(0.03) * dif_color * ao;


				o_color = vec4(result, u_Alpha);
				float gamma = 2.2f;
				o_color.rgb = pow(o_color.rgb, vec3(1.0f/gamma));

			}



		// ----------------------------------------------------------------------------
		vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir, vec4 fragPosLightSpace, int lightIndex)
		{
			vec3 L = normalize(-light.direction);
			vec3 H = normalize(viewDir + L);

			vec3  color     = light.color.rgb * light.intensity;    
			
			// cook-torrance brdf
			float NDF = DistributionGGX(normal, H, roughness);        
			float G   = GeometrySmith(normal, viewDir, L, roughness);      
			vec3 F    = fresnelSchlick(max(dot(H, viewDir), 0.0), F0);       
        
        
			vec3 numerator    = NDF * G * F;
			float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, L), 0.0) + 0.0001f;
			vec3 specular_     = numerator / denominator;  
            
			vec3 kS = F;
			vec3 kD = vec3(1.0) - kS;
			kD *= 1.0 - metallic;	  


			// add to outgoing radiance Lo
			float NdotL = max(dot(normal, L), 0.0);      
			float shadow = ShadowCalculation(fragPosLightSpace, normal, L, lightIndex);
			return (kD * dif_color / PI + specular_) * color * NdotL * shadow; 

		}

		// ----------------------------------------------------------------------------
		vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 fragPosLightSpace)
		{
			vec3 L = normalize(light.position - fragPos);
			vec3 H = normalize(viewDir + L);
			float distance    = length(light.position - fragPos);
			float attenuation =  (1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance))) * light.intensity; 
			vec3 radiance     = light.color.rgb * attenuation;     

			
			// cook-torrance brdf
			float NDF = DistributionGGX(normal, H, roughness);        
			float G   = GeometrySmith(normal, viewDir, L, roughness);      
			vec3 F    = fresnelSchlick(max(dot(H, viewDir), 0.0), F0);       
        
        
			vec3 numerator     = NDF * G * F;
			float denominator  = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, L), 0.0) + 0.0001f;
			vec3 specular_     = numerator / denominator;  
            
			vec3 kS = F;
			vec3 kD = vec3(1.0) - kS;
			kD *= 1.0 - metallic;	  

			//float shadow = ShadowCalculation(fragPosLightSpace, normal, L);

			// add to outgoing radiance Lo
			float NdotL = max(dot(normal, L), 0.0);      
			return (kD * dif_color / PI + specular_) * radiance * NdotL;
		}

		// ----------------------------------------------------------------------------
		vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 fragPosLightSpace, int lightIndex)
		{
			vec3 L = normalize(light.position - fragPos);
			vec3 H = normalize(viewDir + L);
			float distance    = length(light.position - fragPos);
			float attenuation = ( 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance))) * light.intensity; 
			vec3 radiance     = light.color.rgb * attenuation;     

			float theta = dot(L, normalize(-light.direction)); 
			float epsilon = light.cutOff - light.outCutOff;
			float intensity = clamp((theta - light.outCutOff) / epsilon, 0.0, 1.0);
			
			// cook-torrance brdf
			float NDF = DistributionGGX(normal, H, roughness);        
			float G   = GeometrySmith(normal, viewDir, L, roughness);      
			vec3 F    = fresnelSchlick(max(dot(H, viewDir), 0.0), F0);       
        
        
			vec3 numerator     = NDF * G * F;
			float denominator  = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, L), 0.0) + 0.0001f;
			vec3 specular_     = numerator / denominator;  
            
			vec3 kS = F;
			vec3 kD = vec3(1.0) - kS;
			kD *= 1.0 - metallic;	  

			float shadow = ShadowCalculation(fragPosLightSpace, normal, L, lightIndex);

			// add to outgoing radiance Lo
			float NdotL = max(dot(normal, L), 0.0);      
			return (kD * dif_color / PI + specular_) * radiance * NdotL * intensity * shadow; 
		}
	
	// ----------------------------------------------------------------------------
		float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir, int lightIndex)//numero de luz
		{
			vec3 projCoords = (fragPosLightSpace.xyz / fragPosLightSpace.w) * 0.5 + 0.5;
			

			if(projCoords.z > 1.0)
				return 1.0f;


			float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

			float shadow = 0.0f;

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

			// ----------------------------------------------------------------------------
			vec3 getNormalFromMap()
			{
				vec3 tangentNormal = texture(u_normal, texCoord * u_Tiling).xyz * 2.0 - 1.0;

				vec3 Q1  = dFdx(fragPos);
				vec3 Q2  = dFdy(fragPos);
				vec2 st1 = dFdx(texCoord);
				vec2 st2 = dFdy(texCoord);

				vec3 N   = normalize(normal);
				vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
				vec3 B  = -normalize(cross(N, T));
				mat3 TBN = mat3(T, B, N);

				return normalize(TBN * tangentNormal);
			}
			// ----------------------------------------------------------------------------
			float DistributionGGX(vec3 N, vec3 H, float roughness)
			{
				float a = roughness*roughness;
				float a2 = a*a;
				float NdotH = max(dot(N, H), 0.0);
				float NdotH2 = NdotH*NdotH;

				float nom   = a2;
				float denom = (NdotH2 * (a2 - 1.0) + 1.0);
				denom = PI * denom * denom;

				return nom / denom;
			}
			// ----------------------------------------------------------------------------
			float GeometrySchlickGGX(float NdotV, float roughness)
			{
				float r = (roughness + 1.0);
				float k = (r*r) / 8.0;

				float nom   = NdotV;
				float denom = NdotV * (1.0 - k) + k;

				return nom / denom;
			}
			// ----------------------------------------------------------------------------
			float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
			{
				float NdotV = max(dot(N, V), 0.0);
				float NdotL = max(dot(N, L), 0.0);
				float ggx2 = GeometrySchlickGGX(NdotV, roughness);
				float ggx1 = GeometrySchlickGGX(NdotL, roughness);

				return ggx1 * ggx2;
			}
			// ----------------------------------------------------------------------------
			vec3 fresnelSchlick(float cosTheta, vec3 F0)
			{
				return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
			}